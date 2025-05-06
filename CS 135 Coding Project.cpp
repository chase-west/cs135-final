#include <string>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

bool gameOver = false;
bool hasSpecialItem = false; 

struct Room
{
    int id;
    int roomX;
    int roomY;
    string description;
    string roomType;

    void playRoomStory()
    {
        if (roomType == "finish")
        {
            cout << "You have entered the final room!";
            if (hasSpecialItem)
            { 
            gameOver = true;
            }
            else
            {
                cout << "Make sure you get the magic staff to leave!";
            }
        }

        if (roomType == "filler")
        {
            static random_device rd;
            static mt19937 gen(rd());
            uniform_int_distribution<> dist(1, 100);
            int roll = dist(gen);

            if (roll <= 60)
                cout << "You wander into a bland, dust-covered room. No treasure, no monsters—just stale air and empty walls. You shrug and press on.\n";
            else if (roll <= 90)
                cout << "You step into a silent chamber. The walls are cracked, the floor is bare. You hum to yourself and keep moving.\n";
            else
                cout << "This room feels oddly clean, but otherwise unremarkable. You glance around, yawn, and head out.\n";
        }
    }
};

int currentRoomID = 0;
int numberOfRooms = 5;
string roomOptions[4] = { "treasure", "monster", "finish", "filler" };
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

    bool hasItem(string itemName)
    {
        for (int i = 0; i < playerInventory.size(); i++)
            if (itemName == playerInventory[i])
                return true;
        return false;
    }

    void changeLocation(string direction)
    {
        if (direction == "n") playerY += 1;
        else if (direction == "e") playerX += 1;
        else if (direction == "s") playerY -= 1;
        else if (direction == "w") playerX -= 1;

        for (int i = 0; i < rooms.size(); i++)
            if (rooms[i].roomX == playerX && rooms[i].roomY == playerY)
            {
                currentRoomID = i;
                return;
            }

        cout << "No room in that direction! Stay in current room.\n";
        if (direction == "n") playerY -= 1;
        else if (direction == "e") playerX -= 1;
        else if (direction == "s") playerY += 1;
        else if (direction == "w") playerX += 1;
    }
};

Player mainPlayer;
string currentDirection;

void generateRooms()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, 3);

    for (int i = 0; i < numberOfRooms; i++)
    {
        Room room;
        room.id = i;
        room.roomX = i % 3;
        room.roomY = i / 3;
        room.roomType = roomOptions[dist(gen)];
        room.description = "This is a " + room.roomType + " room.";
        rooms.push_back(room);
    }

    mainPlayer.playerX = rooms[0].roomX;
    mainPlayer.playerY = rooms[0].roomY;
}

int main()
{
    generateRooms();

    while (!gameOver)
    {
        rooms[currentRoomID].playRoomStory();
        cout << "You are at (" << mainPlayer.playerX << ", " << mainPlayer.playerY << ")\n";
        cout << "enter direction you wish to travel. (n/e/s/w) ";
        cin >> currentDirection;

        mainPlayer.changeLocation(currentDirection);
    }

    return 0;
}
