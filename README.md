# Spam-Filter
A small spam filter library written in C.

## Installation
To build the library:
```
$ make lib
```
To copy libsf.so to /usr/local/lib:
```
$ sudo make install
```

## Example usage in a FCGI application
This application allows only one request from the same ip address every 5 minutes.
```
...
SpamFilter *filter = SF_AllocFilter(300); // Initialize a filter and set the SpamFilterNode lifetime to 5 minutes
time_t last = time(&last);
time_t current;
char *ip;

if(filter == nullptr)
    // Handle allocation failure
while(FCGI_Accept()) {
    ip = getenv("REMOTE-ADDR");
    if(ip == nullptr)
        FCGI_End();
    // If sufficient time has passed, check each node in the filter for deletion
    if(time(current) - last > filter->time) {
        SF_Delete(filter, current);
        last = current;
    }
    // If an entry was found, close the connection
    if(SF_Find(ip, filter) != nullptr)
        FCGI_End();
    // If an entry wasn't found, add the ip address to the filter
    if(SF_Insert(id, 0, filter) == nullptr) {
        // Handle allocation failure
    }
    // Do something with the received data
    ...
}

SF_FreeFilter(filter);
...
```
