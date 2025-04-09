#include <iostream>
#include <thread>
#include <chrono>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <deque>
#include <string>

using namespace std;

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

deque<float> priceHistory;

int hasMomentum(deque<float>& priceHistory) {
    if (priceHistory.size() == 3) {
        float a = priceHistory[0];
        float b = priceHistory[1];
        float c = priceHistory[2];

        bool up = (a < b) && (b < c);
        bool down = (a > b) && (b > c);

        if (up) return 1;
        if (down) return -1;
        return 0;
    }
    return 0;
}

void receiveAndRespond(int socketFd, const string& name) {
    char buffer[BUFFER_SIZE];

    // Send client name
    send(socketFd, name.c_str(), name.size(), 0);

    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytesReceived = recv(socketFd, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived <= 0) {
            cerr << "Server closed connection or error occurred." << endl;
            break;
        }

        string data(buffer);
        size_t commaPos = data.find(',');
        if (commaPos == string::npos) {
            cerr << "Invalid price format received: " << data << endl;
            continue;
        }

        int priceId = stoi(data.substr(0, commaPos));
        float price = stof(data.substr(commaPos + 1));

        cout << "📥 Received price ID: " << priceId << ", Value: " << price << endl;

        if (priceHistory.size() >= 3) {
            priceHistory.pop_front();
        }
        priceHistory.push_back(price);

        // Check for momentum
        int signal = hasMomentum(priceHistory);

        // Simulate reaction delay
        this_thread::sleep_for(chrono::milliseconds(10 + rand() % 50));

        if (signal) {
            string direction = signal == 1 ? "up" : "down";
            cout << "🚀 Momentum " << direction << "! Sending order for price ID: " << priceId << endl;
            // Send order (price ID)
            string order = to_string(priceId);
            send(socketFd, order.c_str(), order.length(), 0);
            cout << "📤 Sent order for price ID: " << priceId << endl;
        } else {
            cout << "No momentum. Ignoring price ID " << priceId << endl;
        }
    }

    close(socketFd);
}

int main() {
    srand(time(nullptr));

    string name;
    cout << "Enter your client name: ";
    getline(cin, name);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "Socket creation failed!" << endl;
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Connection to server failed!" << endl;
        return 1;
    }

    cout << "✅ Connected to server at " << SERVER_IP << ":" << SERVER_PORT << endl;
    receiveAndRespond(sock, name);
    return 0;
}
