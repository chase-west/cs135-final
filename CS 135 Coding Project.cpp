#include <string>
#include <iostream>
#include <vector>

using namespace std;

struct Room
{
    int id;
    int roomX;
    int roomY;
    string roomType[4] = {"treasure", "monster", "finish", "filler"};
    string description;



    string playRoomStory()
    {
        return "test!";
    }
};

bool gameOver = false;
int currentRoomID = 0;
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

    // main game loop 
    while (!gameOver)
    {
        // render current room 
        Room currentRoom = rooms[currentRoomID];
        cout << rooms[currentRoomID].playRoomStory(); 
        cout << "enter direction you wish to travel. (n/e/s/w)";
        cin >> currentDirection;

        if (currentDirection == "n") {
            mainPlayer.changeLocation("n");
        }

    }
}
