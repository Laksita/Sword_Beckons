#include <cstdlib>
#include <ctype.h>
#include <iostream>
#include <list>
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

// Items in the shop or in the players inventory
class Item {

public:
  Item(string name, int value, int count, int damage) {
    _name = name;
    _value = value;
    _count = count;
    _damage = damage;
  }
  void addOne() { _count++; }
  void removeOne() {
    if (_count > 0) {
      _count--;
    }
  }

  // getters
  string getName() { return _name; }
  int getValue() { return _value; }
  int getCount() { return _count; }
  int getDamage() { return _damage; }

  // setters
  void setCount(int count) { _count = count; }

private:
  string _name;
  int _value;
  int _count;
  int _damage;
};

// Shop where the player can purchase items
class Shop {
public:
  Shop(string name, int money) {
    _name = name;
    _money = money;
  }
  // iterates over the list of Items in the shop and displays them to the screen
  void printShop() {
    cout << "*** Welcome to " << _name << " ***\n\n";
    cout << "Money: " << _money << " GP\n\n";
    list<Item>::iterator lit;

    int i = 1;

    for (lit = _items.begin(); lit != _items.end(); lit++) {
      cout << i << ". " << (*lit).getName() << " x " << (*lit).getCount()
           << " {Price: $" << (*lit).getValue()
           << "} {Damage: " << (*lit).getDamage() << "} " << endl;
      i++;
    }

    cout << endl;
  }

  // checks to see if the shop can afford the Item by comparing their money to
  // the price of the item
  bool canAffordItem(string name, int money) {
    list<Item>::iterator lit;

    for (lit = _items.begin(); lit != _items.end(); lit++) {
      if ((*lit).getName() == name) {
        if ((*lit).getValue() <= money) {
          return true;
        } else {
          return false;
        }
      }
    }
    return false;
  }

  bool purchaseItem(string name, Item &newItem) {
    list<Item>::iterator lit;

    for (lit = _items.begin(); lit != _items.end(); lit++) {
      if ((*lit).getName() == name) {
        newItem = (*lit);

        newItem.setCount(1);
        (*lit).removeOne();

        // removes items of count 0
        if ((*lit).getCount() == 0) {
          _items.erase(lit);
        }
        return true;
      }
    }
    return false;
  }

  void addItem(Item newItem) {
    list<Item>::iterator lit;

    for (lit = _items.begin(); lit != _items.end(); lit++) {
      // if item is in shop already, this increases count and returns
      if ((*lit).getName() == newItem.getName()) {
        (*lit).addOne();
        return;
      }
    }

    // if the item isnt in the shop, add it to the shop
    _items.push_back(newItem);
    _items.back().setCount(1);
  }

  // Function to increase the Player's amount of money
  void addMoney(int money) { _money += money; }

  // getters
  string getName() { return _name; }
  int getMoney() { return _money; }

private:
  string _name;
  list<Item> _items;
  int _money;
};

// Player holds information entered by the user and the results of their actions
class Player {
public:
  Player() {}
  void init(string name, int money) {
    _name = name;
    _money = money;
  }
  string getName() { return _name; }
  void printInventory() {

    cout << "*** " << _name << "'s inventory ***\n\n";
    cout << "Money: " << _money << " GP\n\n";
    list<Item>::iterator lit;

    int i = 1;

    for (lit = _items.begin(); lit != _items.end(); lit++) {
      cout << i << ". " << (*lit).getName() << " x " << (*lit).getCount()
           << "  {Price: $" << (*lit).getValue()
           << "} {Damage: " << (*lit).getDamage() << "} " << endl;
      i++;
    }
  }
  // checks to see if the Player can afford the item by comparing its price to
  // money
  bool canAffordItem(string name, int money) {
    list<Item>::iterator lit;

    for (lit = _items.begin(); lit != _items.end(); lit++) {
      if ((*lit).getName() == name) {
        if ((*lit).getValue() <= money) {
          return true;
        } else {
          return false;
        }
      }
    }
    return false;
  }

  // subtracts Item from the Player's inventory
  bool removeItem(string name, Item &newItem) {
    list<Item>::iterator lit;

    for (lit = _items.begin(); lit != _items.end(); lit++) {
      if ((*lit).getName() == name) {
        newItem = (*lit);
        (*lit).removeOne();
        if ((*lit).getCount() == 0) {
          _items.erase(lit);
        }
        return true;
      }
    }
    return false;
  }
  // adds Item to the Player's inventory
  void addItem(Item newItem) {
    list<Item>::iterator lit;

    for (lit = _items.begin(); lit != _items.end(); lit++) {
      if ((*lit).getName() == newItem.getName()) {
        (*lit).addOne();
        return;
      }
    }
    _items.push_back(newItem);
    _items.back().setCount(1);
  }

  // Adds an integer of money to the Player's stash of money
  void addMoney(int money) { _money += money; }

  // getters
  int getMoney() { return _money; }
  void subtractMoney(int amount) { _money -= amount; }

private:
  string _name;
  list<Item> _items;
  int _money;
};

// initializing void functions
void initShops(list<Shop> &shops);
void initPlayer(Player &player);
void enterShop(Player &player, Shop &shop);

int shopfunction(Player &player, list<Shop> &shops) {
  // iterator
  list<Shop>::iterator lit;
  string shopName;

  // tells us when shopping is over, time to move on
  bool isDone = false;

  while (isDone == false) {
    cout << "Shops:\n\n";
    int i = 1;
    // prints shop names
    for (lit = shops.begin(); lit != shops.end(); lit++) {
      cout << i << ". " << (*lit).getName() << endl;
      i++;
    }

    cout << "\nWhat shop would you like to enter? Enter the name of the shop! "
            "Q to quit: ";
    // get input from user
    cin.ignore(64, '\n');
    cin.clear();
    getline(cin, shopName);

    // if user wants to quit
    if (shopName == "Q" || shopName == "q") {
      isDone = true;
    } else {

      cout << endl;

      bool validShop = false;
      // check to see if the user entered a valid shop
      for (lit = shops.begin(); lit != shops.end(); lit++) {
        if ((*lit).getName() == shopName) {
          enterShop(player, (*lit));
          validShop = true;
        }
      }

      if (validShop == false) {
        cout << "Invalid Shop!\n";
      }
    }
  }
  return 0;
}

// sets up the shops and their items
void initShops(list<Shop> &shops) {
  // item paramaters include (name, value, count, damage)
  shops.push_back(Shop("Ironworks", 100));
  shops.back().addItem(Item("Iron Dagger", 3, 3, 2));
  shops.back().addItem(Item("Iron Sword", 6, 3, 3));
  shops.back().addItem(Item("Iron Spear", 7, 4, 4));
  shops.back().addItem(Item("Iron Axe", 8, 3, 5));
  shops.back().addItem(Item("Iron Greatsword", 10, 2, 6));

  shops.push_back(Shop("Steelworks", 200));
  shops.back().addItem(Item("Steel Dagger", 7, 2, 5));
  shops.back().addItem(Item("Steel Sword", 8, 3, 6));
  shops.back().addItem(Item("Steel Spear", 9, 2, 7));
  shops.back().addItem(Item("Steel Axe", 10, 2, 8));
  shops.back().addItem(Item("Steel Greatsword", 12, 1, 9));
  shops.back().addItem(Item("Steel Mace", 14, 2, 10));

  shops.push_back(Shop("Magicworks", 1500));
  shops.back().addItem(Item("Firesword", 12, 10, 10));
  shops.back().addItem(Item("Frostspear", 12, 5, 12));
  shops.back().addItem(Item("Necrotic Axe", 14, 20, 14));
  shops.back().addItem(Item("Potato", 1, 3, 30));
  shops.back().addItem(Item("Nuclear Archery Kit", 20, 10, 100));
}

// initializes the player
void initPlayer(Player &player) {
  cout << "Enter thy name: ";
  string name;
  getline(cin, name);
  player.init(name, 10);

  player.addItem(Item("Bronze Sword", 5, 1, 2));
  cout << endl;
}

// allows the player to buy and sell items from a shop
void enterShop(Player &player, Shop &shop) {
  bool isDone = false;
  char input;
  string itemName;
  Item newItem("NOITEM", 0, 1, 0);

  while (isDone == false) {
    shop.printShop();
    player.printInventory();

    cout << "\nWould you like to buy or sell? Q to quit. (B/S): ";
    cin >> input;
    cout << endl;
    cin.ignore(64, '\n');
    cin.clear();

    // if the user wants to leave the shop
    if (input == 'Q' || input == 'q')
      return;

    if (input == 'B' || input == 'b') { // buying
      cout << "Enter the item you wish to buy: ";
      getline(cin, itemName);

      if (shop.canAffordItem(itemName, player.getMoney())) {

        if (shop.purchaseItem(itemName, newItem) == true) {
          player.addMoney(-newItem.getValue());
          player.addItem(newItem);
          shop.addMoney(newItem.getValue());
        } else {
          cout << "That is an invalid item!\n";
          system("PAUSE");
        }
      } else {
        cout << "You don't have enough money!\n";
        system("PAUSE");
      }

    } else { // selling
      cout << "Enter the item you wish to sell: ";
      getline(cin, itemName);

      if (player.canAffordItem(itemName, shop.getMoney())) {

        if (player.removeItem(itemName, newItem) == true) {
          shop.addMoney(-newItem.getValue());
          shop.addItem(newItem);
          player.addMoney(newItem.getValue());
        } else {
          cout << "That is an invalid item!\n";
          system("PAUSE");
        }
      } else {
        cout << "The shop doesn't have enough money!\n";
        system("PAUSE");
      }
    }
  }
}

// Basic printout function to help seperate the text output
void seperator() { cout << "---------------------------------" << endl; }

// Acts as a gameover screen when the player dies and exits the program
void gameover() {
  seperator();
  seperator();
  cout << "\tGAME OVER" << endl;
  seperator();
  seperator();
  exit(0);
}

// First path in the story split that allows the user to pick their path
string menu() {
  seperator();
  string holder = "";
  cout << "1: Enter the red door" << endl
       << "2: Enter the green door" << endl
       << "3: Enter the blue door" << endl;
  cin >> holder;

  return holder;
}

// The first part of the story where the player can enter the red door to fight
// a goblin (1/2 chance of victory), enter the green door to open a chest(gain 5
// gold), or enter the blue door (random chance of finding 0-4 dollars)
void story(Player player) {
  bool chest = true;
  list<Shop> shops;
  cout << player.getName()
       << " descends into the darkness of the tomb, ready to face whatever "
          "lies ahead"
       << endl;
  string holder = menu();
  int chance;
  while (true) {
    if (holder == "1") {
      cout << "You encountered a goblin!" << endl;
      chance = rand() % 2 + 1;
      if (chance == 1) {
        player.addMoney(3);
        cout << "You killed the goblin and got 3 dollars!" << endl;
        seperator();
        break;
      }

      else {
        cout << "The goblin smacks you and you black out. It was a cointoss' "
                "chance. "
                "Your adventure ends here."
             << endl;
        gameover();
      }

    }

    else if (holder == "2") {
      if (chest == true) {
        cout << "There is an open golden chest with a shiny scroll in front of "
                "you."
             << endl
             << "You shred the scroll and gain 5 dollars" << endl;
        player.addMoney(5);
        chest = false;
        holder = menu();
      } else {
        cout << "The empty chest is still here, so you turn around and leave"
             << endl;
        holder = menu();
      }
    }

    else if (holder == "3") {
      chance = rand() % 4;
      cout << "You walk into an empty room that has infinite piles of garbage "
              "in it"
           << endl;
      cin.ignore();
      cout << "You scrounge around and find " << chance
           << " dollars. You quietly tuck them into your bag and turn around "
           << endl;
      player.addMoney(chance);
      holder = menu();
    } else {
      cout << "You spin around in a circle and keep debating on what door "
              "to go through"
           << endl;
      holder = menu();
    }
  }

  cout << "You enter a watery room with a well. There is a ghost that wants "
          "you to retrieve it's locket"
       << endl;
  seperator();

  cout << "1: Help the ghost" << endl << "2: Don't help the ghost" << endl;
  cin >> holder;

  if (holder == "1") {
    cout << "You attempt to reach down the well to get the locket" << endl;
    chance = rand() % 5 + 1;
    if (chance <= 4) {
      cout << "You successfully get the locket and return it to the ghost"
           << endl
           << "It rewards you with 5 dollars!" << endl;
      player.addMoney(5);
      seperator();
    } else {
      cout << "You fell to your death and perished. It was a 1/5 chance."
           << endl;
      gameover();
    }
  } else {
    cout << "The ghost wails, but you walk on by" << endl;
    seperator();
  }
}

// Basic introduction to get the player immersed into the world
void intro() {
  cout << "Welcome to the Sword Beckons!"
       << " press enter to continue..." << endl;
  cin.ignore();
  cout << "Stranger: Hey you! You're finally awake!" << endl;
  cin.ignore();
  cout << "Stranger: Woah that is some blow to the head you got "
          "there...here...let me help you up."
       << endl;
  cin.ignore();
  cout << "Stranger: There you are, back on your feet! What's your name "
          "traveler? (Press enter!)"
       << endl;
  cin.ignore();
}

// Second path in the story split that allows the user to pick their path
string menu2() {
  seperator();
  string holder = "";
  cout << "1: Go down the left path that's emitting a foul smell" << endl
       << "2: Go down the right path that's littered with bones" << endl
       << "3: Enter the shopping district that's tucked away in the corner"
       << endl;
  cin >> holder;

  return holder;
}

// The second last part of the story where the player can fight an ogre (1/2 chance of victory, a skeleton (2/3 chance of victory), or enter a shop 
void story2(Player player) {
  list<Shop> shops;
  initShops(shops);
  cout << "As you turn"
      "down the next corner,"
      "you have another choice to make." << endl;
  string holder = menu2();

  while (true) {
    if (holder == "1") {
      cout << "You encounter an ogre!" << endl;
      int chance = rand() % 2 + 1;
      if (chance == 1) {
        player.addMoney(5);
        cout << "You killed the ogre and got 5 dollars!" << endl;
        seperator();
        break;
      }

      else {
        cout << "The ogre wallops you and you black out. It was a cointoss 1/3"
                "chance. "
                "Your adventure ends here."
             << endl;
        gameover();
      }
      break;
    } else if (holder == "2") {
      cout << "You encounter a skeleton!" << endl;
      int chance = rand() % 3 + 1;
      if (chance <= 2) {
        player.addMoney(3);
        cout << "You killed the skeleton and got 3 dollars!" << endl;
        seperator();
        break;
      }

      else {
        cout << "The skeleton obliterates you and you black out. It was a 1/3 "
                "chance. "
                "Your adventure ends here."
             << endl;
        gameover();
      }
      break;
    } else if (holder == "3") {
      cout << endl
           << "You enter the shop and ask to upgrade your sword"
           << endl;
      shopfunction(player, shops);
      cout << "You leave the shop feeling somewhat more capitalistic than "
              "before..."
           << endl;
      story2(player);
      break;
    } 
    else {
      cout << "You stare blankly at the wall trying to decide what to do"
           << endl;
      story2(player);
    }
  }
}

// The last part of the story where the player must enter the combination of red
//green blue to proceed and fight the boss 
void story3(Player player) {
  string temp;
  int count = 0;

  cout << "After that battle, you enter a long room with a tiled floor."
       << endl;

  cin.ignore();

  cout << "On the other side of the room, there's a large stone door with an "
          "extravagent lock"
       << endl;

  cin.ignore();

  cout << "Upon inspecting the door, you find a simple message etched onto the "
          "doorframe"
       << endl;

  cin.ignore();

  cout << "Enter here the prismatic sequence or be reduced to dullness" << endl;

  cin.ignore();

  cout << "What do you enter first?" << endl;
  cin >> temp;

  if (temp == "red" || temp == "Red") {
    count++;
  }
  cout << endl << "What do you enter second?" << endl;
  cin >> temp;

  if (temp == "green" || temp == "Green") {
    count++;
  }

  cout << endl << "What do you enter third?" << endl;
  cin >> temp;

  if (temp == "blue" || temp == "Blue") {
    count++;
  }

  seperator();
  if (count == 3) {
    cout << "The door slides open and the corpse of Raga'thur appears!" << endl;
    // fight

    cout << "He says: congratulations adventurer! You've beaten the game!"
         << endl;

    cout << player.getName() << " completed the adventure with "
         << player.getMoney() << " dollars!" << endl;

    exit(0);
  } else {
    cout << "You realize that was the incorrect color sequence as the tile "
            "drops out beneath your feet"
         << endl;

    cin.ignore();

    cout << endl
         << "You fall down into the abyss and the void consumes you" << endl;

    gameover();
  }
}

// main method that seeds the run, initializes the player, calls intro and story
// methods
int main() {
  Player player;
  intro();

  initPlayer(player);
  
  srand(time(0));

  cout << "Stranger: I don't mean to be the bearer"          
          "of bad news or anything "
       << player.getName() << "..." << endl;
  cin.ignore();
  cout << "Stranger: But you lost your sword." 
      << endl;
  cin.ignore();
  cout << "Stranger: Hey...here's a rusty one, go"           
          "back into the tomb when "
          "you're ready."
       << endl;
  cin.ignore();

  story(player);

  story2(player);

  story3(player);
}
