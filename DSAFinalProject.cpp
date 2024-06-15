#include <bits/stdc++.h>
using namespace std;
string networkLocations[] = {"NUST", "SADDAR", "FAIZABAD", "RAWAL CHOWK", "WESTRIDGE", "I8", "LOK VIRSA", "G-9", "FAST", "F-9", "26-number"};
class Graph
{
public:
    // Dijkstra's Algorithm to find the shortest path from source S
    vector<int> dijkstra(int V, vector<pair<int, int>> adj[], int S)
    {
        set<pair<int, int>> st;
        vector<int> dist(V, INT_MAX);

        dist[S] = 0;
        st.insert({0, S});

        while (!st.empty())
        {
            auto it = *(st.begin());
            st.erase(it);
            int node = it.second;
            int dis = it.first;

            for (auto it : adj[node])
            {
                int adjNode = it.first;
                int edgW = it.second;

                if (dis + edgW < dist[adjNode])
                {
                    if (dist[adjNode] != INT_MAX)
                        st.erase({dist[adjNode], adjNode});

                    dist[adjNode] = dis + edgW;
                    st.insert({dist[adjNode], adjNode});
                }
            }
        }
        return dist;
    }

    // Prim's Algorithm to find Minimum Spanning Tree (MST)
    int spanningTree(int V, vector<pair<int, int>> adj[])
    {
        set<pair<int, pair<int, int>>> st;
        vector<int> vis(V, 0);
        vector<int> parent(V, -1);
        vector<int> dis(V, 1e9);
        st.insert({0, {0, -1}});
        int sum = 0;

        while (!st.empty())
        {
            auto it = *(st.begin());
            st.erase(it);
            int node = it.second.first;
            int wt = it.first;
            int par = it.second.second;

            if (vis[node] == 1)
                continue;

            vis[node] = 1;
            dis[node] = wt;
            sum += wt;
            parent[node] = par;

            for (auto &adjNodeInfo : adj[node])
            {
                int adjNode = adjNodeInfo.first;
                int edW = adjNodeInfo.second;
                if (!vis[adjNode])
                {
                    st.insert({edW, {adjNode, node}});
                }
            }
        }

        cout << "Edges in the MST with their weights:" << endl;
        for (int i = 0; i < V; ++i)
        {
            if (parent[i] != -1)
            {
                cout << "Edge: " << networkLocations[parent[i]] << " - " << networkLocations[i] << " Weight: " << dis[i] << endl;
            }
        }

        return sum;
    }
};

struct Package
{
    string name;
    double price;
    int speed;     // in Mbps
    int dataLimit; // in GB
};

Package packages[3] = {
    {"PREMIUM", 100.0, 100, 1000},
    {"BUSINESS", 70.0, 50, 500},
    {"STUDENT", 40.0, 20, 200}};

int totalCustomers = 0;

struct Customer
{
    string name;
    int package;
    int id;
    bool paid = 1;
    bool blocked = 0;
    Customer *left = 0;
    Customer *right = 0;
    Customer() {}
    Customer(string _name, int _package)
    {
        ++totalCustomers;
        name = _name;
        package = _package;
        id = totalCustomers;
    }
};

Customer *root = 0;

struct CompareCustomers
{
    bool operator()(Customer &c1, Customer &c2)
    {
        return c1.package > c2.package;
    }
};

priority_queue<Customer, vector<Customer>, CompareCustomers> connections;
priority_queue<Customer, vector<Customer>, CompareCustomers> complains;

int max(int a, int b)
{
    return (a > b) ? a : b;
}

int getHeight(Customer *node)
{
    if (node == nullptr)
        return 0;

    int lHeigt = getHeight(node->left);
    int RHeigt = getHeight(node->right);

    return max(lHeigt, RHeigt) + 1;
}

int getBalance(Customer *node)
{
    if (node == nullptr)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

Customer *rotateRight(Customer *y)
{
    Customer *x = y->left;
    Customer *T2 = x->right;

    x->right = y;
    y->left = T2;

    return x;
}

Customer *rotateLeft(Customer *x)
{
    Customer *y = x->right;
    Customer *T2 = y->left;

    y->left = x;
    x->right = T2;

    return y;
}

void requestNewConnection()
{
    string name;
    int package;
    cout << "Enter your name" << endl;
    cin >> name;
    cout << "Choose a Package" << endl;
    cout << "\n1. PREMIUM" << endl;
    cout << "2. BUSINESS" << endl;
    cout << "3. STUDNET" << endl;

    cin >> package;
    --package;
    if (package == 1 || package == 2 || package == 0)
    {
        connections.push(Customer(name, package));
        cout << "Request added Successfully\nNow wait for Admin approval" << endl;
        cout << "Your ID is : " << totalCustomers << endl;
    }
    else
    {
        cout << "Invalid choice" << endl;
        cout << "Request not added" << endl;
    }
}

Customer *insertCustomer(Customer *root, Customer *newNode)
{
    if (root == nullptr)
    {
        return newNode;
    }

    if (newNode->id < root->id)
        root->left = insertCustomer(root->left, newNode);
    else if (newNode->id > root->id)
        root->right = insertCustomer(root->right, newNode);
    else
        return root;

    int balance = getBalance(root);

    if (balance > 1 && newNode->id < root->left->id)
        return rotateRight(root);

    if (balance < -1 && newNode->id > root->right->id)
        return rotateLeft(root);

    if (balance > 1 && newNode->id > root->left->id)
    {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    if (balance < -1 && newNode->id < root->right->id)
    {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

void acceptCustomerRequest()
{
    if (connections.size() == 0)
    {
        cout << "no connection requests\n first customers have to request for new connection form sign up and then admin can accept their request and provide them with new connection" << endl;
        return;
    }
    string name = connections.top().name;
    int package = connections.top().package;
    int id = connections.top().id;
    Customer *cus = new Customer();
    cus->name = name;
    cus->package = package;
    cus->id = id;

    cout << packages[cus->package].name << " connection provided to ID: " << cus->id << ", Name: " << cus->name << endl;

    root = insertCustomer(root, cus);
    connections.pop();
}

Customer *findMinValueNode(Customer *node)
{
    Customer *current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

Customer *deleteCustomer(Customer *root, int id)
{
    if (root == nullptr)
        return root;

    if (id < root->id)
        root->left = deleteCustomer(root->left, id);
    else if (id > root->id)
        root->right = deleteCustomer(root->right, id);
    else
    {
        if (root->left == nullptr && root->right == nullptr)
        {
            delete root;
            root = nullptr;
        }
        else if (root->left != nullptr && root->right == nullptr)
        {
            Customer *temp = root->left;
            delete root;
            root = temp;
        }
        else if (root->left == nullptr && root->right != nullptr)
        {
            Customer *temp = root->right;
            delete root;
            root = temp;
        }
        else if (root->left != nullptr && root->right != nullptr)
        {
            int min = findMinValueNode(root->right)->id;
            root->id = min;
            root->right = deleteCustomer(root->right, min);
        }
    }

    if (root == nullptr)
        return root;

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rotateRight(root);

    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return rotateLeft(root);

    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

void inOrderTraversal(Customer *root)
{
    if (root != 0)
    {
        inOrderTraversal(root->left);
        cout << "ID: " << root->id << ", Name: " << root->name << ", Package: " << packages[root->package].name << ", Paid: " << (root->paid ? "Yes" : "No") << ", Blocked: " << (root->blocked ? "Yes" : "No") << endl;
        inOrderTraversal(root->right);
    }
}

void blockUnpaid(Customer *root)
{
    if (root != nullptr)
    {
        blockUnpaid(root->left);
        if (root->paid == false)
        {
            root->blocked = true;
        }
        blockUnpaid(root->right);
    }
}

void setPaidStatusFalse(Customer *root)
{
    if (root != nullptr)
    {
        setPaidStatusFalse(root->left);
        root->paid = false;
        setPaidStatusFalse(root->right);
    }
}

Customer *search(Customer *root, int id)
{
    if (root == NULL || root->id == id)
        return root;

    if (root->id < id)
        return search(root->right, id);

    return search(root->left, id);
}

void payBills(Customer *root, int id)
{
    if (root == NULL)
        return;

    Customer *result = search(root, id);
    if (result == NULL)
    {
        cout << "Customer not found!" << endl;
    }
    else
    {
        result->paid = true;
        cout << "Bill paid for customer ID: " << result->id << endl;
    }
}

void registerComplain(int id)
{
    Customer c = *search(root, id);
    complains.push(c);
    cout << "Complain registered successfully" << endl;
}

// Processing complaints in order of package priority
void processComplaints(priority_queue<Customer, vector<Customer>, CompareCustomers> &complains)
{
    if (complains.empty())
    {
        cout << "No complaints to process." << endl;
        return;
    }
    cout << endl;
    while (!complains.empty())
    {
        Customer c = complains.top();
        complains.pop();
        cout << "Complaint from customer ID: " << c.id << ", Name: " << c.name << ", Package: " << packages[c.package].name << " is solved" << endl;
    }
}

void changePackage(Customer *root, int id)
{

    if (root == NULL)
        return;

    if (root->id == id)
    {
        int p;
        cout << "Choose a Package" << endl;
        cout << "\n1. PREMIUM" << endl;
        cout << "2. BUSINESS" << endl;
        cout << "3. STUDNET" << endl;
        cin >> p;
        --p;
        root->package = p;
    }

    if (root->id < id)
        return changePackage(root->right, id);

    return changePackage(root->left, id);
}

void viewAndUpdatePackages()
{
    cout << "\n--- Package Details ---\n";
    for (int i = 0; i < 3; ++i)
    {
        cout << "Package " << i << ": " << packages[i].name << "\n";
        cout << "Price: $" << packages[i].price << " per month\n";
        cout << "Speed: " << packages[i].speed << " Mbps\n";
        cout << "Data Limit: " << packages[i].dataLimit << " GB\n";
    }

    cout << "\nDo you want to update any package? (yes/no): ";
    string choice;
    cin >> choice;

    if (choice == "yes")
    {
        int pkg;
        cout << "Enter package number to update (0-2): ";
        cin >> pkg;

        if (pkg < 0 || pkg > 2)
        {
            cout << "Invalid package number!" << endl;
            return;
        }

        cout << "Enter new price: ";
        cin >> packages[pkg].price;
        cout << "Enter new speed (Mbps): ";
        cin >> packages[pkg].speed;
        cout << "Enter new data limit (GB): ";
        cin >> packages[pkg].dataLimit;

        cout << "Package updated successfully!" << endl;
    }
}

void adminmenu()
{
    cout << "\n--- ISP Admin Menu ---\n";
    cout << "1. Accept New Connection Request\n";
    cout << "2. Block Unpaid Customers\n";
    cout << "3. View All Customers\n";
    cout << "4. Process Complaints\n";
    cout << "5. Set All Customers as Unpaid\n";
    cout << "6. Shortest Path (Dijkstra)\n";
    cout << "7. Minimum Spanning Tree (Prim's)\n";
    cout << "8. View and Update Packages\n";
    cout << "0. Back\n";
    cout << "Enter your choice: ";
}

void userMenu()
{
    cout << "\n<<< ISP User Menu >>>"
         << "\n\n1. Register Complain"
         << "\n2. Pay Bills"
         << "\n3. Change Package"
         << "\n\nSelect Option: ";
}

int main()
{
    Graph graphAlgos;
    const int V = 11;
    vector<pair<int, int>> adj[V];

    adj[0].push_back({1, 1});
    adj[1].push_back({0, 1});
    adj[1].push_back({2, 1});
    adj[2].push_back({1, 1});
    adj[2].push_back({3, 1});
    adj[3].push_back({2, 1});
    adj[3].push_back({4, 1});
    adj[4].push_back({3, 1});
    adj[4].push_back({5, 1});
    adj[5].push_back({4, 1});
    adj[5].push_back({6, 1});
    adj[6].push_back({5, 1});
    adj[6].push_back({7, 1});
    adj[7].push_back({6, 1});
    adj[7].push_back({8, 1});
    adj[8].push_back({7, 1});
    adj[8].push_back({9, 1});
    adj[9].push_back({8, 1});
    adj[9].push_back({10, 1});
    adj[10].push_back({9, 1});

    int opt;
    do
    {
        cout << "1. Enter as admin" << endl;
        cout << "2. Enter as User" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter: ";
        cin >> opt;
        if (opt != 1 && opt != 2 && opt != 0)
        {
            cout << "Invalid option:" << endl;
            continue;
        }
        else if (opt == 1)
        {
            int choice;
            while (true)
            {
                adminmenu();
                cin >> choice;

                if (choice == 0)
                    break;

                switch (choice)
                {
                case 1:
                {
                    acceptCustomerRequest();
                    break;
                }
                case 2:
                {
                    blockUnpaid(root);
                    break;
                }
                case 3:
                {
                    inOrderTraversal(root);
                    break;
                }
                case 4:
                {
                    processComplaints(complains);
                    break;
                }
                case 5:
                {
                    setPaidStatusFalse(root);
                    break;
                }
                case 6:
                {
                    int start;
                    cout << "Enter starting location (0-10): ";
                    cin >> start;
                    vector<int> dist = graphAlgos.dijkstra(V, adj, start);
                    cout << "Shortest distances from location " << networkLocations[start] << ":\n";
                    for (int i = 0; i < V; ++i)
                        cout << "To " << networkLocations[i] << ": " << dist[i] << "\n";
                    break;
                }
                case 7:
                {
                    int mstWeight = graphAlgos.spanningTree(V, adj);
                    cout << "Total weight of MST: " << mstWeight << "\n";
                    break;
                }
                case 8:
                {
                    viewAndUpdatePackages();
                    break;
                }
                default:
                    cout << "Invalid choice! Please try again.\n";
                }
            }
        }
        else if (opt == 2)
        {
            int choice;
            do
            {
                cout << "1. Sign in" << endl;
                cout << "2. Sign up" << endl;
                cout << "0. Go Back" << endl;
                cin >> choice;
                if (choice != 1 && choice != 2 && choice != 0)
                {
                    cout << "Invalid choice" << endl;
                    continue;
                }
                else if (choice == 1)
                {
                    int CustomerID;
                    cout << "Enter your id : ";
                    cin >> CustomerID;
                    if (search(root, CustomerID) == 0)
                    {
                        cout << "\nno such id exists\n";
                        continue;
                    }
                    int option;
                    userMenu();
                    switch (option)
                    {
                    case 1:
                        registerComplain(CustomerID);
                        break;
                    case 2:
                        payBills(root, CustomerID);
                        break;
                    case 3:
                        changePackage(root, CustomerID);
                        break;
                    default:
                        cout << "Invalid Option" << endl;
                        break;
                    }
                }
                else if (choice == 2)
                {
                    requestNewConnection();
                }

            } while (choice != 0);
        }
    } while (opt != 0);

    return 0;
}
