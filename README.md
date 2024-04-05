# resource-sharing-system

This is a simple resource management system implemented in C++ to manage resources in a hostel setting, such as calculators, books, etc. It allows users to reserve resources for specific dates and times efficiently and provides functionalities for adding, updating, and deleting resources.

# Features
* Add Resource: Add a new resource to the system with its name, description, and quantity.
* Delete Resource: Remove a resource from the system based on its name.
* Update Resource: Modify the details of an existing resource, such as its name, description, or quantity.
* Reserve Resource (FCFS): Reserve a resource using the First Come First Serve (FCFS) algorithm, ensuring fairness in resource allocation.
* Reserve Resource (LRU): Reserve a resource using the Least Recently Used (LRU) algorithm, optimizing resource allocation based on usage patterns.
* Cancel Reservation: Cancel a previously made reservation.
* Notify User: Notify a user about their reservation status or any other relevant information.
* Display Resources: View all available resources along with their details.
* Rate Resource: Allow users to rate resources they have used, helping to improve resource quality.
* View All Reservations: View all reservations made in the system.