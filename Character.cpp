/*#include "Character.h"

Characterx::Characterx(string description) {
	this->description = description;
}
void Characterx::addItem(Item &item) {
    itemsInCharacter.push_back(item);
}
void Characterx::addItem(Item *item) {
    itemsInCharacter.push_back(*item);
    delete item;
}
string Characterx::longDescription()
{
  string ret = this->description;
  ret += "\n Item list:\n";
  for (vector<Item>::iterator i = itemsInCharacter.begin(); i != itemsInCharacter.end(); i++)
    ret += "\t"+ (*i).getLongDescription() + "\n";
  return ret;
}
*/


