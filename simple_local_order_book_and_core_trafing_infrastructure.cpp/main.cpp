#include "feed_parser.h"
#include "order_manager.h"
#include "market_snapshot.h"

int main() {
    auto feed = load_feed("sample_feed.txt");

    OrderManager order_manager;
    MarketSnapshot snapshot;

    for (const auto& event : feed) {
        event.print();
        
        if (event.type == FeedType::BID) {
            snapshot.update_bid(event.price, event.quantity);
            order_manager.place_order(Side::Buy, event.price, event.quantity);
        } else if (event.type == FeedType::ASK) {
            snapshot.update_ask(event.price, event.quantity);
            order_manager.place_order(Side::Sell, event.price, event.quantity);
        } else if (event.type == FeedType::EXECUTION) {
            order_manager.handle_fill(event.order_id, event.quantity);
        }
    }

    return 0;
}