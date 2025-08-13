#include <bits/stdc++.h>
using namespace std;

struct Room {
    int roomNo;
    string type;
    double rate;
    bool booked;
    string guestName;
    string checkIn;
    string checkOut;
};

struct MenuItem {
    int id;
    string name;
    double price;
};

struct OrderItem {
    int menuId;
    int qty;
};

struct Booking {
    int bookingId;
    int roomNo;
    string guestName;
    string checkIn;
    string checkOut;
    vector<OrderItem> orders;
    double totalBill;
};

class Hotel {
    vector<Room> rooms;
    vector<MenuItem> menu;
    vector<Booking> bookings;
    int nextBookingId;
public:
    Hotel(): nextBookingId(1) {
        loadRooms();
        loadMenu();
        loadBookings();
    }
    void run() {
        while (true) {
            cout << "\n=== Hotel Management System ===\n";
            cout << "1. View Rooms\n2. View Menu\n3. Book Room\n4. Order Food (existing booking)\n5. Checkout & Bill\n6. View Bookings\n7. Save Now\n8. Exit\n";
            cout << "Enter choice: ";
            int ch; if (!(cin >> ch)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (ch==1) displayRooms();
            else if (ch==2) displayMenu();
            else if (ch==3) createBooking();
            else if (ch==4) addFoodToBooking();
            else if (ch==5) checkoutBooking();
            else if (ch==6) displayBookings();
            else if (ch==7) { saveRooms(); saveBookings(); cout << "Saved.\n"; }
            else if (ch==8) { saveRooms(); saveBookings(); cout << "Goodbye.\n"; break; }
            else cout << "Invalid.\n";
        }
    }
private:
    void ensureSampleFiles() {
        ifstream fr("rooms.csv");
        if (!fr.good()) {
            ofstream fw("rooms.csv");
            fw << "RoomNo,Type,Rate,Booked,GuestName,CheckIn,CheckOut\n";
            fw << "101,Single,1200.00,0,,,\n";
            fw << "102,Single,1200.00,0,,,\n";
            fw << "201,Double,2000.00,0,,,\n";
            fw << "202,Double,2000.00,0,,,\n";
            fw << "301,Deluxe,3500.00,0,,,\n";
            fw << "302,Deluxe,3500.00,0,,,\n";
            fw.close();
        }
        ifstream fm("menu.csv");
        if (!fm.good()) {
            ofstream fw("menu.csv");
            fw << "ID,Name,Price\n";
            fw << "1,Tea,30.00\n";
            fw << "2,Coffee,40.00\n";
            fw << "3,Sandwich,120.00\n";
            fw << "4,Pasta,200.00\n";
            fw << "5,Steak,650.00\n";
            fw.close();
        }
        ifstream fb("bookings.csv");
        if (!fb.good()) {
            ofstream fw("bookings.csv");
            fw << "BookingId,RoomNo,GuestName,CheckIn,CheckOut,Orders,TotalBill\n";
            fw.close();
        }
    }
    vector<string> split(const string &s, char delim) {
        vector<string> out;
        string cur;
        for (char c: s) {
            if (c==delim) { out.push_back(cur); cur.clear(); }
            else cur.push_back(c);
        }
        out.push_back(cur);
        return out;
    }
    void loadRooms() {
        ensureSampleFiles();
        rooms.clear();
        ifstream f("rooms.csv");
        string line;
        getline(f, line);
        while (getline(f, line)) {
            if (line.size()==0) continue;
            vector<string> flds = split(line, ',');
            Room r;
            r.roomNo = stoi(flds[0]);
            r.type = flds[1];
            r.rate = stod(flds[2]);
            r.booked = (flds.size()>3 && flds[3]=="1");
            r.guestName = (flds.size()>4?flds[4]:"");
            r.checkIn = (flds.size()>5?flds[5]:"");
            r.checkOut = (flds.size()>6?flds[6]:"");
            rooms.push_back(r);
        }
    }
    void saveRooms() {
        ofstream f("rooms.csv");
        f << "RoomNo,Type,Rate,Booked,GuestName,CheckIn,CheckOut\n";
        for (auto &r: rooms) {
            f << r.roomNo << "," << r.type << "," << fixed << setprecision(2) << r.rate << ",";
            f << (r.booked? "1": "0") << ",";
            f << escapeCsv(r.guestName) << "," << escapeCsv(r.checkIn) << "," << escapeCsv(r.checkOut) << "\n";
        }
    }
    string escapeCsv(const string &s) {
        string out = s;
        if (out.find(',')!=string::npos || out.find('"')!=string::npos) {
            string tmp;
            tmp.push_back('"');
            for (char c: out) {
                if (c=='"') tmp += "\"\"";
                else tmp.push_back(c);
            }
            tmp.push_back('"');
            return tmp;
        }
        return out;
    }
    void loadMenu() {
        menu.clear();
        ifstream f("menu.csv");
        string line;
        getline(f,line);
        while (getline(f,line)) {
            if (line.size()==0) continue;
            vector<string> flds = split(line, ',');
            MenuItem m;
            m.id = stoi(flds[0]);
            m.name = flds[1];
            m.price = stod(flds[2]);
            menu.push_back(m);
        }
    }
    void displayRooms() {
        cout << "\nRooms:\n";
        cout << "No\tType\tRate\tStatus\tGuest\tCheckIn\tCheckOut\n";
        for (auto &r: rooms) {
            cout << r.roomNo << "\t" << r.type << "\t" << fixed << setprecision(2) << r.rate << "\t";
            cout << (r.booked? "Booked":"Available") << "\t" << r.guestName << "\t" << r.checkIn << "\t" << r.checkOut << "\n";
        }
    }
    void displayMenu() {
        cout << "\nMenu:\n";
        cout << "ID\tName\tPrice\n";
        for (auto &m: menu) cout << m.id << "\t" << m.name << "\t" << fixed << setprecision(2) << m.price << "\n";
    }
    Room* findRoom(int roomNo) {
        for (auto &r: rooms) if (r.roomNo==roomNo) return &r;
        return nullptr;
    }
    void createBooking() {
        displayRooms();
        cout << "Enter room number to book: ";
        int rn; if (!(cin >> rn)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); return; }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        Room* r = findRoom(rn);
        if (!r) { cout << "Invalid room.\n"; return; }
        if (r->booked) { cout << "Room already booked.\n"; return; }
        cout << "Guest name: "; string name; getline(cin, name);
        cout << "Check-in date (YYYY-MM-DD): "; string ci; getline(cin, ci);
        cout << "Check-out date (YYYY-MM-DD): "; string co; getline(cin, co);
        r->booked = true;
        r->guestName = name;
        r->checkIn = ci;
        r->checkOut = co;
        Booking b;
        b.bookingId = nextBookingId++;
        b.roomNo = r->roomNo;
        b.guestName = name;
        b.checkIn = ci;
        b.checkOut = co;
        b.orders.clear();
        b.totalBill = computeRoomCharge(*r, ci, co);
        bookings.push_back(b);
        saveRooms();
        saveBookings();
        cout << "Booked. Booking ID: " << b.bookingId << " Total room charge: " << fixed << setprecision(2) << b.totalBill << "\n";
    }
    double computeRoomCharge(const Room &r, const string &ci, const string &co) {
        int nights = dateDiff(ci, co);
        if (nights<=0) nights = 1;
        double charge = nights * r.rate;
        return charge;
    }
    int dateDiff(const string &a, const string &b) {
        tm ta = {}, tb = {};
        if (!parseDate(a, ta) || !parseDate(b, tb)) return 1;
        time_t ta_t = mktime(&ta);
        time_t tb_t = mktime(&tb);
        double diff = difftime(tb_t, ta_t);
        int days = (int)round(diff / (60*60*24));
        return days>0?days:1;
    }
    bool parseDate(const string &s, tm &out) {
        int y=0, m=0, d=0;
        if (sscanf(s.c_str(), "%d-%d-%d", &y, &m, &d) != 3) return false;
        out.tm_year = y - 1900;
        out.tm_mon = m - 1;
        out.tm_mday = d;
        out.tm_hour = 12;
        out.tm_min = 0;
        out.tm_sec = 0;
        return true;
    }
    void loadBookings() {
        bookings.clear();
        ifstream f("bookings.csv");
        if (!f.good()) { nextBookingId = 1; return; }
        string line;
        getline(f, line);
        int maxId = 0;
        while (getline(f, line)) {
            if (line.size()==0) continue;
            vector<string> flds = split(line, ',');
            Booking b;
            b.bookingId = stoi(flds[0]);
            b.roomNo = stoi(flds[1]);
            b.guestName = flds[2];
            b.checkIn = flds[3];
            b.checkOut = flds[4];
            b.orders.clear();
            string ordersEncoded = flds[5];
            if (ordersEncoded.size()>0) {
                vector<string> items = split(ordersEncoded, ';');
                for (auto &it: items) {
                    if (it.size()==0) continue;
                    vector<string> kv = split(it, ':');
                    OrderItem oi; oi.menuId = stoi(kv[0]); oi.qty = stoi(kv[1]);
                    b.orders.push_back(oi);
                }
            }
            b.totalBill = stod(flds[6]);
            bookings.push_back(b);
            if (b.bookingId > maxId) maxId = b.bookingId;
        }
        nextBookingId = maxId + 1;
        for (auto &r: rooms) {
            for (auto &b: bookings) {
                if (r.roomNo == b.roomNo) {
                    r.booked = true;
                    r.guestName = b.guestName;
                    r.checkIn = b.checkIn;
                    r.checkOut = b.checkOut;
                }
            }
        }
    }
    void saveBookings() {
        ofstream f("bookings.csv");
        f << "BookingId,RoomNo,GuestName,CheckIn,CheckOut,Orders,TotalBill\n";
        for (auto &b: bookings) {
            f << b.bookingId << "," << b.roomNo << "," << escapeCsv(b.guestName) << "," << b.checkIn << "," << b.checkOut << ",";
            string enc;
            for (auto &oi: b.orders) {
                if (!enc.empty()) enc.push_back(';');
                enc += to_string(oi.menuId) + ":" + to_string(oi.qty);
            }
            f << enc << "," << fixed << setprecision(2) << b.totalBill << "\n";
        }
    }
    void displayBookings() {
        if (bookings.empty()) { cout << "No bookings.\n"; return; }
        cout << "\nBookings:\n";
        cout << "BID\tRoom\tGuest\tCheckIn\tCheckOut\tTotal\n";
        for (auto &b: bookings) {
            cout << b.bookingId << "\t" << b.roomNo << "\t" << b.guestName << "\t" << b.checkIn << "\t" << b.checkOut << "\t" << fixed << setprecision(2) << b.totalBill << "\n";
        }
    }
    Booking* findBookingById(int id) {
        for (auto &b: bookings) if (b.bookingId==id) return &b;
        return nullptr;
    }
    MenuItem* findMenuItem(int id) {
        for (auto &m: menu) if (m.id==id) return &m;
        return nullptr;
    }
    void addFoodToBooking() {
        displayBookings();
        cout << "Enter booking id: ";
        int bid; if (!(cin >> bid)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); return; }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        Booking* b = findBookingById(bid);
        if (!b) { cout << "Invalid booking.\n"; return; }
        displayMenu();
        while (true) {
            cout << "Enter menu id to add (0 to stop): ";
            int mid; if (!(cin >> mid)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); break; }
            if (mid==0) { cin.ignore(numeric_limits<streamsize>::max(), '\n'); break; }
            MenuItem* mi = findMenuItem(mid);
            if (!mi) { cout << "Invalid menu id.\n"; continue; }
            cout << "Qty: ";
            int q; if (!(cin >> q)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); break; }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            bool found=false;
            for (auto &oi: b->orders) if (oi.menuId==mid) { oi.qty += q; found=true; break; }
            if (!found) { OrderItem oi; oi.menuId = mid; oi.qty = q; b->orders.push_back(oi); }
            double add = mi->price * q;
            b->totalBill += add;
            cout << "Added " << q << " x " << mi->name << " (" << fixed << setprecision(2) << add << ")\n";
        }
        saveBookings();
        cout << "Orders updated. Total: " << fixed << setprecision(2) << b->totalBill << "\n";
    }
    void checkoutBooking() {
        displayBookings();
        cout << "Enter booking id to checkout: ";
        int bid; if (!(cin >> bid)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); return; }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        int idx=-1;
        for (int i=0;i<(int)bookings.size();++i) if (bookings[i].bookingId==bid) { idx=i; break; }
        if (idx==-1) { cout << "Not found.\n"; return; }
        Booking b = bookings[idx];
        cout << "\n--- Invoice ---\n";
        cout << "Booking ID: " << b.bookingId << "\nRoom No: " << b.roomNo << "\nGuest: " << b.guestName << "\n";
        cout << "CheckIn: " << b.checkIn << " CheckOut: " << b.checkOut << "\n";
        cout << "Room Charge: " << fixed << setprecision(2) << computeRoomCharge(*findRoom(b.roomNo), b.checkIn, b.checkOut) << "\n";
        cout << "Food Orders:\n";
        for (auto &oi: b.orders) {
            MenuItem* mi = findMenuItem(oi.menuId);
            if (mi) cout << mi->name << " x" << oi.qty << " = " << fixed << setprecision(2) << (mi->price * oi.qty) << "\n";
        }
        cout << "Total Bill: " << fixed << setprecision(2) << b.totalBill << "\n";
        cout << "Proceed to payment? (y/n): ";
        char c; cin >> c; cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (c=='y' || c=='Y') {
            for (auto &r: rooms) if (r.roomNo==b.roomNo) { r.booked=false; r.guestName=""; r.checkIn=""; r.checkOut=""; break; }
            bookings.erase(bookings.begin()+idx);
            saveRooms();
            saveBookings();
            cout << "Payment done. Checked out.\n";
        } else cout << "Checkout cancelled.\n";
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Hotel h;
    h.run();
    return 0;
}
