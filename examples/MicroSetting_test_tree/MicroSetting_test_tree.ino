#include <MicroOscSlip.h>
#include <MicroSetting.h>
#include <Preferences.h>

THIS EXAMPLE DOES NOT WORK AND WILL NOT COMPILE
IT IS ONLY A MODEL ON HOW I WOULD LIKE THINS TO WORK

MicroOscSlip<128> myOsc(&Serial);
Preferences preferences;
MicroSetting intSetting = MicroSetting("intValue", 0, 255, 127);

enum labeledSettingEnum { ABC, DEF, GHI };
const char* labeledSettingLabels[3] = { "ABC", "DEF", "GHI"};
MicroSetting labeledSetting = MicroSetting("Type", labeledSettingLabels, SIZE_OF_ARRAY(labeledSettingLabels));

MicroSetting * groupArray[2] = { &intSetting, &labeledSetting };
MicroSettingGroup groupSetting = MicroSettingGroup("Group", groupArray, SIZE_OF_ARRAY(groupArray));

#define NODE_STRING_LEN 16
typedef struct {
    Node* parent;
    const char name[NODE_STRING_LEN];
    MicroSetting* setting;
    //int depth;
} Node;

Node rootNode = {NULL, "", NULL};
#define MAX_NODE_COUNT 256
Node nodeArray[MAX_NODE_COUNT];
size_t nodeArrayIdx = 0;
#define ADDR_BUFFER_SIZE 128
char addrBuffer[ADDR_BUFFER_SIZE];

void attachSettingToNode(const char* address, MicroSetting* setting) {
    // tonkenize address
    // iterate over nodeArray
    // match parent address
    // add nodes as needed
    size_t length = strlen(address);
    if(length > ADDR_BUFFER_SIZE-1){
        Serial.print("Address too long : ");
        Serial.println(address);
        return;
    }
    // the -1 and +1 has to do with strtok string termination or something
    strncpy(addrBuffer, address, length+1);
    char * split = strtok(addrBuffer, "/");
    Node * currentNode = &rootNode;
    while (split != NULL) {
        if(strlen(split) > NODE_STRING_LEN) {
            Serial.println("Node name too long");
            return;
        }
        bool found = false;
        for (int i = 0; i < MAX_NODE_COUNT; i++) {
            if (nodeArray[i].parent == currentNode && strcmp(nodeArray[i].name, split) == 0) {
                currentNode = &nodeArray[i];
                found = true;
                break;
            }
        }
        if (!found) {
            nodeArray[nodeArrayIdx].parent = currentNode;
            currentNode = nodeArray[nodeArrayIdx++];
            if(nodeArrayIdx >= MAX_NODE_COUNT){
                Serial.println("ran out of nodes");
                return;
            }
            strcpy(currentNode->name, split);
        }
        split = strtok(NULL, "/");
        if(split == NULL){
            currentNode->setting = setting;
            Serial.println("Setting attached");
            return;
        }
    }
}

void dispatchOSC(MicroOscMessage & oscMessage) {
    // look for 
    size_t length = strlen(address);
    if(length > ADDR_BUFFER_SIZE+1){
        Serial.print("Address too long : ");
        Serial.println(address);
        return;
    }
    oscMessage.copyAddress(addrBuffer, length+1);
    // strncpy(addrBuffer, address, length+1);
    char * split = strtok(addrBuffer, "/");
    Node * currentNode = &rootNode;
    while (split != NULL) {
        bool found = false;
        for (int i = 0; i < MAX_NODE_COUNT; i++) {
            if (nodeArray[i].parent == currentNode) {
                if(strcmp(nodeArray[i].name, split) == 0){
                    currentNode = &nodeArray[i];
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            Serial.print("Node not found : ");
            Serial.println(split);
            return;
        }
        split = strtok(NULL, "/");
        if(split == NULL){
            // last node
            Serial.println("found setting");
            // deserialize according to data type
            return;
        }
    }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
//   rootNode.parent = &rootNode;
  groupSetting.restoreEachFrom(&preferences);
}

void loop() {
  // put your main code here, to run repeatedly:
   myOsc.onOscMessageReceived(dispatchOSC);
   delay(50);
}
