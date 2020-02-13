#ifndef USER_HPP
# define USER_HPP

# include <map>
# include <string>
# include "unit.hpp"

class User {
private:
	std::string	_key;
	std::string	_name;
public:
	User(std::string key = "");
	~User();
	std::string	getKey() const;
	std::string	getName() const;
	void		setKey(std::string key);
	void		setName(std::string name);
	int			auth;
	UnitManager						unitMan;
};

class UserManager {
private:
	std::map<std::string, User*>	_users;
public:
	UserManager();
	~UserManager();
	void	addUser(std::string key);
	void	removeUser(std::string key);
	User*	getUser(std::string key);
};

#endif
