#include <map>

using namespace std;
enum class Side { Buy, Sell };
enum class OrderStatus { New, Filled, PartiallyFilled, Cancelled };

struct MyOrder {
    int id;
    Side side;
    double price;
    int quantity;
    int filled = 0;
    OrderStatus status = OrderStatus::New;

    MyOrder(int id_, Side s, double p, int q)
        : id(id_), side(s), price(p), quantity(q) {}
};

class OrderManager {
    private:
        map<int, unique_ptr<MyOrder>> orders;
        long long int order_id = 0;
    public:
        int place_order(Side side, double price, int qty) {
            order_id++;
            orders[order_id] = make_unique<MyOrder>(order_id, side, price, qty);
            return order_id;
        }
        void cancel(int id) {
            if (orders.find(id) != orders.end()) {
                cout << "Canceling order " << id << endl;
                orders[id]->status = OrderStatus::Cancelled;
            } else {
                cout << "Canceling order, but order id " << id << " does not exist" << endl;
            }
        }
        void handle_fill(int id, int filled_qty) {
            if (orders.find(id) != orders.end()) {
                cout << "Filling order " << id << " with quantity " << filled_qty << endl;
                orders[id]->filled += filled_qty;
                if (orders[id]->filled == orders[id]->quantity) {
                    cout << "Order " << id << " completely filled" << endl;
                    orders.erase(id);
                    cout << "Order " << id << " deleted" << endl;
                } else if (orders[id]->filled < orders[id]->quantity && orders[id]->filled > 0) {
                    orders[id]->status = OrderStatus::PartiallyFilled;
                    cout << "Order " << id << " partially filled" << endl;
                }
            } else {
                cout << "Filling order, but order id " << id << " does not exist" << endl;
            }
        }
        void print_active_orders() const {
            for (const auto& order : orders) {
                if (order.second->status == OrderStatus::New || order.second->status == OrderStatus::PartiallyFilled) {
                    cout << "Order " << order.first << " is active with price " << order.second->price << " and quantity " << order.second->quantity << endl;
                }
            }
        }
};