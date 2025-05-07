#include <string>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

bool gameOver = false;

struct Room
{
    int id;
    int roomX;
    int roomY;
    bool hasKey = false;
    bool hasKeyFinder = false;
    string description;
    string roomType;

    void playRoomStory()
    {
        if (roomType == "finish")
        {
            cout << "You arrive in the escape room! There is a giant glowing door begging for your escape!" << endl;
        }
        else if (roomType == "treasure")
        {
            cout << "You've found a treasure room! Shiny gold and jewels glimmer in the torchlight." << endl;
        }
        else if (roomType == "monster")
        {
            cout << "A fearsome monster lurks in the shadows of this room. You carefully navigate around it." << endl;
        }
        else if (roomType == "filler")
        {
            static random_device rd;
            static mt19937 gen(rd());
            uniform_int_distribution<> dist(1, 100);
            int roll = dist(gen);

            if (roll <= 60)
                cout << "You wander into a bland, dust-covered room. No treasure, no monsters just stale air and empty walls. You shrug and press on.\n";
            else if (roll <= 90)
                cout << "You step into a silent chamber. The walls are cracked, the floor is bare. You hum to yourself and keep moving.\n";
            else
                cout << "This room feels oddly clean, but otherwise unremarkable. You glance around, yawn, and head out.\n";
        }
    }
};

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> dist(5, 10);

int currentRoomID = 0;
int numberOfRooms = dist(gen);
string roomOptions[2] = { "filler", "monster" };
vector<Room> rooms;

struct Player
{
    int playerX;
    int playerY;

    vector<string> playerInventory;

    void addItem(string itemName)
    {
        playerInventory.push_back(itemName);
    }

    string getItem(int inventorySpot)
    {
        if (inventorySpot >= 1 && inventorySpot <= playerInventory.size()) {
            return playerInventory[inventorySpot - 1];
        }
        return ""; 
    }

    void displayInventory()
    {
        if (playerInventory.size() != 0)
        {
            cout << "This is your inventory!" << endl;
            for (int i = 0; i < playerInventory.size(); i++)
            {
                cout << i + 1 << ". " << playerInventory[i] << endl;
            }
        }
        else
        {
            cout << "Your inventory is empty!" << endl;
        }
    }

    bool hasItem(string itemName)
    {
        for (int i = 0; i < playerInventory.size(); i++)
            if (itemName == playerInventory[i])
                return true;
        return false;
    }

    bool useItem(int itemPosition)
    {
        if (itemPosition >= 1 && itemPosition <= playerInventory.size()) {
            playerInventory.erase(playerInventory.begin() + itemPosition - 1);
            return true;
        }
        return false; 
    }

    void changeLocation(string direction)
    {
        int oldX = playerX;
        int oldY = playerY;

        if (direction == "n") playerY += 1;
        else if (direction == "e") playerX += 1;
        else if (direction == "s") playerY -= 1;
        else if (direction == "w") playerX -= 1;
        else {
            cout << "Invalid direction! Please enter n, e, s, or w." << endl;
            return;
        }

        bool roomFound = false;
        for (int i = 0; i < rooms.size(); i++) {
            if (rooms[i].roomX == playerX && rooms[i].roomY == playerY) {
                currentRoomID = i;
                roomFound = true;
                break;
            }
        }

        if (!roomFound) {
            cout << "No room in that direction! Stay in current room.\n";
            playerX = oldX;
            playerY = oldY;
        }
    }
};

Player mainPlayer;
string currentDirection;

bool createKeyFinder()
{
    // 25% chance to make keyFinder
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, 3);
    return (dist(gen) == 0);
}

void generateRooms()
{
    // gen random index to guarantee treasure and final room
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> idxDist(0, numberOfRooms - 1);

    int treasureIndex = idxDist(gen);
    int finishIndex;
    do {
        finishIndex = idxDist(gen);
    } while (finishIndex == treasureIndex);

    int keyPlacement;
    // gen random room for key
    do {
        keyPlacement = idxDist(gen);
    } while ((keyPlacement == finishIndex) || (keyPlacement == treasureIndex));

    // gen other rooms randomly 
    uniform_int_distribution<> dist(0, 1);

    for (int i = 0; i < numberOfRooms; i++)
    {
        Room room;
        room.id = i;
        room.roomX = i % 3;
        room.roomY = i / 3;

        if (i == treasureIndex)
        {
            room.roomType = "treasure";
        }
        else if (i == finishIndex)
        {
            room.roomType = "finish";
        }
        else
        {
            room.roomType = roomOptions[dist(gen)];
        }

        // assign keyFinder to any room except where the key is
        if (i != keyPlacement) {
            room.hasKeyFinder = createKeyFinder();
        }

        if (keyPlacement == i)
        {
            room.hasKey = true;
        }

        room.description = "This is a " + room.roomType + " room.";
        rooms.push_back(room);
    }

    mainPlayer.playerX = rooms[0].roomX;
    mainPlayer.playerY = rooms[0].roomY;
}

// testing function to verify room structure
void displayRoomTypes()
{
    for (int i = 0; i < rooms.size(); i++)
    {
        cout << rooms[i].description << endl;
    }
}

void useKeyFinder(int& keyX, int& keyY) {
    for (int i = 0; i < rooms.size(); i++) {
        if (rooms[i].hasKey == true) {
            keyX = rooms[i].roomX;
            keyY = rooms[i].roomY;
            return;
        }
    }
    keyX = -1;
    keyY = -1;
}

int main()
{
    generateRooms();
    cout << "You enter a mystic castle with " << rooms.size() << " rooms!" << endl;
    cout << "Escape before you die!" << endl;

    while (!gameOver)
    {
        rooms[currentRoomID].playRoomStory();

        // check for key finder in current room
        if (rooms[currentRoomID].hasKeyFinder == true)
        {
            cout << "You have collected a key finder! Open inventory to use!" << endl;
            mainPlayer.addItem("keyFinder");
            rooms[currentRoomID].hasKeyFinder = false; 
        }

        // check if current room has the key
        if ((rooms[currentRoomID].hasKey) && (!mainPlayer.hasItem("key")))
        {
            cout << "This room has the key!" << endl;
            cout << "Type (h) to collect!" << endl;
            char input;
            cin >> input;
            if (input != 'h')
            {
                do
                {
                    cout << "Please collect the key by entering h!" << endl;
                    cin >> input;
                } while (input != 'h');
            }
            cout << "You have collected the key!" << endl;
            mainPlayer.addItem("key");
        }

        // check if in finish room with key
        if (rooms[currentRoomID].roomType == "finish")
        {
            if (mainPlayer.hasItem("key"))
            {
                cout << "You have the key! Press k to escape!" << endl;
                char input;
                cin >> input;
                if (input == 'k')
                {
                    cout << "You have escaped!" << endl;
                    gameOver = true; 
                    break;
                }
            }
            else
            {
                cout << "Return with the key to escape!" << endl;
            }
        }

        string userInput;
        cout << "You are at (" << mainPlayer.playerX << ", " << mainPlayer.playerY << ")\n";
        cout << "Enter direction you wish to travel (n/e/s/w) or type inv to open your inventory!" << endl;
        cin >> userInput;

        if (userInput == "inv")
        {
            mainPlayer.displayInventory();

            if (mainPlayer.playerInventory.empty()) {
                continue; // skip if inventory is empty
            }

            int invNumber;
            cout << "Enter the number of the item you want to use! (ex. 1): ";
            cin >> invNumber;

            // Check if the selected item is valid
            if (invNumber < 1 || invNumber > mainPlayer.playerInventory.size()) {
                cout << "Invalid item number!" << endl;
                continue;
            }

            string itemToUse = mainPlayer.getItem(invNumber);

            if (itemToUse == "keyFinder") {
                int keyX, keyY;
                useKeyFinder(keyX, keyY);
                random_device rd;
                mt19937 gen(rd());
                uniform_int_distribution<> dist(0, 1);

                if (dist(gen) == 0) {
                    cout << "The x coordinate of the key is: " << keyX << "!" << endl;
                }
                else {
                    cout << "The y coordinate of the key is: " << keyY << "!" << endl;
                }

                mainPlayer.useItem(invNumber);
            }
            else if (itemToUse == "key")
            {
                cout << "You can't use your key until you escape!" << endl;
            }


            else {
                cout << "You used " << itemToUse << "." << endl;
                mainPlayer.useItem(invNumber);
            }
        }
        else
        {
            mainPlayer.changeLocation(userInput);
        }
    }

    return 0;
}