#include <boost/asio.hpp>
#include <charconv>
#include <array>
#include <vector>
#include <string_view>
#include <cstring>
#include <iostream>

using boost::asio::ip::udp;
using boost::asio::ip::tcp;

static constexpr char GROUP[]      = "239.255.0.1";
static constexpr unsigned short PORT      = 3001;
static constexpr unsigned short TCP_PORT  = 4000;
static constexpr char NAME[]      = "ToddTan";

static constexpr int MAX_SEC      = 10000;
static constexpr int BUF_SZ       = 4096;

struct ChallengeCtx {
    int  id{-1};
    int  target_idx{-1};
    std::array<double,MAX_SEC+1> bid{}, ask{};
    std::array<uint8_t,MAX_SEC+1> seen{};
    std::vector<int> touched;

    // true when we know id, target, and have at least one quote for target
    bool ready() const noexcept {
        return id>0 && target_idx>0 && seen[target_idx];
    }
    // zero-cost reset (only clear touched entries)
    void reset() noexcept {
        for(int idx : touched) seen[idx]=0;
        touched.clear();
        id       = -1;
        target_idx = -1;
    }
};


// convert “SECNNNN” to int NNNN  (no allocations / branches)
static inline int ticker_to_index(std::string_view t) noexcept {
    int idx=0;
    std::from_chars(t.data()+3,t.data()+t.size(),idx);   // skip “SEC”
    return idx;
}

// parse double via strtod (≈3× faster than stringstream, from_chars(fp) not yet wide-spread)
static inline double fast_atod(const char* begin,const char* end) noexcept {
    char buf[32];
    size_t len = static_cast<size_t>(end-begin);
    memcpy(buf,begin,len);
    buf[len]='\0';
    return std::strtod(buf,nullptr);
}

int main(){
    boost::asio::io_context io;

    /* ----- UDP multicast listener ----- */
    udp::socket usock(io);
    udp::endpoint listen_ep(udp::v4(),PORT);
    usock.open(listen_ep.protocol());
    usock.set_option(udp::socket::reuse_address(true));
    usock.bind(listen_ep);
    usock.set_option(boost::asio::ip::multicast::join_group(
                     boost::asio::ip::make_address(GROUP)));

    usock.set_option(boost::asio::socket_base::receive_buffer_size(1<<20));

    tcp::socket tsock(io);
    tsock.connect({boost::asio::ip::address_v4::loopback(),TCP_PORT});
    tsock.set_option(boost::asio::ip::tcp::no_delay(true));

    ChallengeCtx ctx;
    std::string rb;              // rolling buffer for reassembly

    /* ----- hot receive loop ----- */
    char pkt[BUF_SZ];
    for(;;){
        udp::endpoint sender;
        std::size_t n = usock.receive_from(boost::asio::buffer(pkt),sender,0);
        rb.append(pkt,n);

        std::size_t nl;
        while((nl = rb.find('\n')) != std::string::npos){
            std::string_view line(rb.c_str(),nl);
            rb.erase(0,nl+1);

            if(line.rfind("SEC|",0)==0){
                // format: SEC|SEC0007|BID|92.9962|ASK|93.761
                // cheap tokenisation by scanning for ‘|’
                const char* p=line.data()+4;                 // after “SEC|”
                const char* q=strchr(p,'|');
                int idx = ticker_to_index({p,(std::size_t)(q-p)});
                const char* bid_start = q+5;                 // skip “|BID|”
                const char* bid_end   = strchr(bid_start,'|');
                const char* ask_start = bid_end+5;           // skip “|ASK|”
                const char* ask_end   = line.data()+line.size();

                ctx.bid[idx] = fast_atod(bid_start,bid_end);
                ctx.ask[idx] = fast_atod(ask_start,ask_end);

                if(!ctx.seen[idx]) { ctx.seen[idx]=1; ctx.touched.push_back(idx); }

            } else if(line.rfind("CHALLENGE_ID:",0)==0){
                ctx.id = std::stoi(std::string(line.substr(13)));
            } else if(line.rfind("TARGET:",0)==0){
                ctx.target_idx = ticker_to_index(line.substr(7));
            }

            if(ctx.ready()){
                int idx = ctx.target_idx;
                char out[128];
                int len = std::snprintf(out,sizeof(out),
                         "CHALLENGE_RESPONSE %d SEC%04d %.10f %.10f %s\n",
                         ctx.id,idx,ctx.bid[idx],ctx.ask[idx],NAME);

                boost::asio::write(tsock,boost::asio::buffer(out,len));
                std::cout<<"▶ sent "<<out; 
                ctx.reset();
            }
        }
    }
}
