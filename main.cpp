#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <unordered_map>

using namespace std;

class Resource
{
private:
    string name;
    string description;
    int quantity;
    bool available;

public:
    Resource(string n, string desc, int qty) : name(n), description(desc), quantity(qty), available(true) {}

    string getName() const { return name; }
    string getDescription() const { return description; }
    int getQuantity() const { return quantity; }
    bool isAvailable() const { return available; }

    void updateAvailability(bool status) { available = status; }
};

class Reservation
{
private:
    string userId;
    string resourceId;
    time_t startTime;
    time_t endTime;

public:
    Reservation(string user, string resource, time_t start, time_t end) : userId(user), resourceId(resource), startTime(start), endTime(end) {}

    string getUserId() const { return userId; }
    string getResourceId() const { return resourceId; }
    time_t getStartTime() const { return startTime; }
    time_t getEndTime() const { return endTime; }
};

class ResourceManager
{
private:
    vector<Resource> resources;
    map<string, Reservation> reservations;
    unordered_map<string, string> notifications;   // userId -> message
    unordered_map<string, double> resourceRatings; // resourceId -> averageRating

public:
    void addResource(const Resource &res) { resources.push_back(res); }
    void deleteResource(const string &name)
    {
        for (auto it = resources.begin(); it != resources.end(); ++it)
        {
            if (it->getName() == name)
            {
                resources.erase(it);
                break;
            }
        }
    }
    void updateResource(const string &name, const Resource &updatedRes)
    {
        for (auto &resource : resources)
        {
            if (resource.getName() == name)
            {
                // Update resource attributes
                resource = updatedRes;
                break;
            }
        }
    }

    void reserveResource_FCFS(const string &userId, const string &resourceId, time_t startTime, time_t endTime)
    {
        for (auto &resource : resources)
        {
            if (resource.getName() == resourceId && resource.isAvailable())
            {
                // Make reservation
                Reservation reservation(userId, resourceId, startTime, endTime);
                reservations[userId] = reservation;

                // Update resource availability
                resource.updateAvailability(false);

                // Notify user
                string message = "Reservation confirmed for " + resourceId + " from " + ctime(&startTime) + " to " + ctime(&endTime);
                notifications[userId] = message;

                cout << "Reservation successful. Details sent to user." << endl;
                return;
            }
        }
        cout << "Resource not available for reservation." << endl;
    }

    void reserveResource_LRU(const string &userId, const string &resourceId, time_t startTime, time_t endTime)
    {
        time_t minLastReservationTime = time(nullptr); // Initialize to current time
        Resource *selectedResource = nullptr;

        for (auto &resource : resources)
        {
            if (resource.getName() == resourceId && resource.isAvailable())
            {
                // Check last reservation time for the resource
                auto it = reservations.find(resourceId);
                time_t lastReservationTime = (it != reservations.end()) ? it->second.getStartTime() : minLastReservationTime;

                // Check if current resource is least recently used
                if (lastReservationTime < minLastReservationTime)
                {
                    minLastReservationTime = lastReservationTime;
                    selectedResource = &resource;
                }
            }
        }

        if (selectedResource)
        {
            // Make reservation
            Reservation reservation(userId, resourceId, startTime, endTime);
            reservations[userId] = reservation;

            // Update resource availability
            selectedResource->updateAvailability(false);

            // Notify user
            string message = "Reservation confirmed for " + resourceId + " from " + ctime(&startTime) + " to " + ctime(&endTime);
            notifications[userId] = message;

            cout << "Reservation successful. Details sent to user." << endl;
        }
        else
        {
            cout << "Resource not available for reservation." << endl;
        }
    }

    void cancelReservation(const string &userId)
    {
        auto it = reservations.find(userId);
        if (it != reservations.end())
        {
            // Get resource ID from reservation
            string resourceId = it->second.getResourceId();

            // Update resource availability
            for (auto &resource : resources)
            {
                if (resource.getName() == resourceId)
                {
                    resource.updateAvailability(true);
                    break;
                }
            }

            // Remove reservation
            reservations.erase(userId);
            cout << "Reservation cancelled successfully." << endl;
        }
        else
        {
            cout << "No reservation found for the user." << endl;
        }
    }

    void notifyUser(const string &userId)
    {
        auto it = notifications.find(userId);
        if (it != notifications.end())
        {
            cout << "Notification for user " << userId << ": " << it->second << endl;
        }
        else
        {
            cout << "No notifications for the user." << endl;
        }
    }

    void displayResources()
    {
        cout << "Available Resources:" << endl;
        for (const auto &resource : resources)
        {
            cout << "Name: " << resource.getName() << endl;
            cout << "Description: " << resource.getDescription() << endl;
            cout << "Quantity: " << resource.getQuantity() << endl;
            cout << "Available: " << (resource.isAvailable() ? "Yes" : "No") << endl;
            cout << endl;
        }
    }

    void rateResource(const string &resourceId, double rating)
    {
        resourceRatings[resourceId] = rating;
    }

    double getAverageRating(const string &resourceId)
    {
        auto it = resourceRatings.find(resourceId);
        if (it != resourceRatings.end())
        {
            return it->second;
        }
        return 0.0;
    }

    void viewAllReservations()
    {
        cout << "All Reservations:" << endl;
        for (const auto &pair : reservations)
        {
            const Reservation res = pair.second;
            cout << "User: " << res.getUserId() << ", Resource: " << res.getResourceId() << endl;
        }
    }
};

int main()
{
    ResourceManager manager;
    int choice = 0;

    while (choice != 11)
    {
        cout << "\nMenu:\n";
        cout << "1. Add Resource\n";
        cout << "2. Delete Resource\n";
        cout << "3. Update Resource\n";
        cout << "4. Reserve Resource (FCFS)\n";
        cout << "5. Reserve Resource (LRU)\n";
        cout << "6. Cancel Reservation\n";
        cout << "7. Notify User\n";
        cout << "8. Display Resources\n";
        cout << "9. Rate Resource\n";
        cout << "10. View All Reservations\n";
        cout << "11. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            string name, description;
            int quantity;
            cout << "Enter resource name: ";
            cin >> name;
            cout << "Enter resource description: ";
            cin >> description;
            cout << "Enter resource quantity: ";
            cin >> quantity;
            manager.addResource({name, description, quantity});
            break;
        }
        case 2:
        {
            string name;
            cout << "Enter resource name to delete: ";
            cin >> name;
            manager.deleteResource(name);
            break;
        }
        case 3:
        {
            string name, newName, newDescription;
            int newQuantity;
            cout << "Enter resource name to update: ";
            cin >> name;
            cout << "Enter new resource name: ";
            cin >> newName;
            cout << "Enter new resource description: ";
            cin >> newDescription;
            cout << "Enter new resource quantity: ";
            cin >> newQuantity;
            manager.updateResource(name, {newName, newDescription, newQuantity});
            break;
        }
        case 4:
        {
            string userId, resourceId;
            time_t startTime, endTime;
            cout << "Enter user ID: ";
            cin >> userId;
            cout << "Enter resource ID to reserve: ";
            cin >> resourceId;
            startTime = time(nullptr);
            endTime = startTime + 3600; // 1 hour reservation
            manager.reserveResource_FCFS(userId, resourceId, startTime, endTime);
            break;
        }
        case 5:
        {
            string userId, resourceId;
            time_t startTime, endTime;
            cout << "Enter user ID: ";
            cin >> userId;
            cout << "Enter resource ID to reserve: ";
            cin >> resourceId;
            startTime = time(nullptr);
            endTime = startTime + 3600; // 1 hour reservation
            manager.reserveResource_LRU(userId, resourceId, startTime, endTime);
            break;
        }
        case 6:
        {
            string userId;
            cout << "Enter user ID to cancel reservation: ";
            cin >> userId;
            manager.cancelReservation(userId);
            break;
        }
        case 7:
        {
            string userId;
            cout << "Enter user ID to notify: ";
            cin >> userId;
            manager.notifyUser(userId);
            break;
        }
        case 8:
        {
            manager.displayResources();
            break;
        }
        case 9:
        {
            string resourceId;
            double rating;
            cout << "Enter resource ID to rate: ";
            cin >> resourceId;
            cout << "Enter rating (0.0 - 5.0): ";
            cin >> rating;
            manager.rateResource(resourceId, rating);
            break;
        }
        case 10:
        {
            manager.viewAllReservations();
            break;
        }
        case 11:
        {
            cout << "Exiting program.\n";
            break;
        }
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
