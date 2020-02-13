#include "../../includes/user.hpp"

User::User(std::string key)
{
	_key = key;
	_name = "UNKNOWN";
	auth = 0;
}
std::string	User::getKey() const { return _key; }
std::string User::getName() const { return _name; }
void		User::setKey(std::string key) { _key = key; }
void		User::setName(std::string name) { _name = name; }
User::~User() { return ; }

void	UserManager::removeUser(std::string key)
{
	delete _users[key];
	_users.erase(key);
}
UserManager::UserManager() { return ; }
UserManager::~UserManager() { return ; }
void	UserManager::addUser(std::string key) {_users.insert(std::make_pair(key, new User(key)));}
User*	UserManager::getUser(std::string key) { return (_users[key]); }
