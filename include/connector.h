
typedef struct _Connector Connector;


Connector* newConnector(const char* hostname, const char* port);
void freeConnector(Connector* connector);

void connectToServer(Connector* connector);
void disconnectFromServer(Connector* connector);
void initiateProlog(Connector *connector);