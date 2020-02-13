#ifndef UNIT_HPP
# define UNIT_HPP

# include <string>
# include <map>
# include <vector>

enum UnitTypes {
	BUILDER,
	FIGHTER
};

enum Directions {
	NORTH,
	SOUTH,
	EAST,
	WEST
};

enum States {
	IDLE,
	MOVING,
	BUILDING,
	FIGHTING
};

class Unit {
private:
	size_t		_type;
	int			_hp;
	int			_x;
	int			_y;
	int			_state;
	size_t		_id;
public:
	Unit(size_t type = 0, size_t id = 0);
	~Unit();
	void	move(int dir);
	void	damage(int amt);
	void	heal(int amt);
	void	build(int type);
	size_t	getType();
	int		getHp();
	int		getX();
	int		getY();
	int		getState();
	size_t	getId();
	void	setX(int x);
	void	setY(int y);
};

class UnitManager {
private:
	std::map<std::string, Unit*>				_units;
	std::map<std::string, std::pair<int, int>>	_commands;
	size_t	_unitCount;
public:
	UnitManager();
	~UnitManager();
	void	addUnit(int type);
	void	submitCommand(std::string id, int action, int arg);
	Unit	*getUnit(std::string id);
	void	removeUnit(std::string id);
	void	processCommands(void);
};

#endif
