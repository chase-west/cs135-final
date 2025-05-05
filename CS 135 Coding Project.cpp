#include <string>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

bool gameOver = false;

struct Room
{
    int id;
    int roomX;
    int roomY;
    string description;
    string roomType;



    void playRoomStory()
    {
        if (roomType == "treasure")
        {
            // play treasure story
        }
        else if (roomType == "monster")
        {
            // play monster story
        }
        else if (roomType == "filler")
        {
            // do filler room! 
        }
        else if (roomType == "finish")
        {
            // do finish room
            gameOver = true;
        }
    }
};


int currentRoomID = 0;
int numberOfRooms = 5; // change this to be random in future
string roomOptions[4] = { "treasure", "monster", "finish", "filler" };
vector<Room> rooms;

void generateRooms()
{

    random_device rd; 
    mt19937 gen(rd());

    int min = 0;
    int max = 3;

    uniform_int_distribution<> dist(min, max);

    // Generate a random number
    int randomNum = dist(gen);

    for (int i = 0; i < numberOfRooms; i++)
    {
        Room room;
        room.id = i;
        room.roomType = roomOptions[randomNum];
        rooms.push_back(room); // push room into vector of rooms
    }
}

void generateRooms();

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
        {
            if (itemName == playerInventory[i])
            {
                return true;
            }
        }
        return false;
    }

    void changeLocation(string direction)
    {
        // move to next room based on rooms near current one 

        // convert direction into number

        if (direction == "n")
        {
            playerY += 1;
        }

        else if (direction == "e")
        {
            playerX += 1;
        }

        else if (direction == "s")
        {
            playerY -= 1;
        }
        else if (direction == "w")
        {
            playerX -= 1;
        }
    }
};

Player mainPlayer;
string currentDirection;


bool canSwitchRoom()
{
    if (mainPlayer.playerX == rooms[currentRoomID].roomX && mainPlayer.playerY == rooms[currentRoomID].roomY)
    {
        return true;
    }
    
    else
    {   
        return false; 
    }
}

int main()
{ 
    generateRooms();

    // main game loop 
    while (!gameOver)
    {
        // render current room 
        Room currentRoom = rooms[currentRoomID];
        rooms[currentRoomID].playRoomStory(); 
        cout << "enter direction you wish to travel. (n/e/s/w)";
        cin >> currentDirection;

        if (currentDirection == "n") {
            mainPlayer.changeLocation("n");
        }

    }
}


