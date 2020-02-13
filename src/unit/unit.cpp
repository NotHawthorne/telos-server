#include "../../includes/server.hpp"

Unit::Unit(size_t type, size_t id)
{
	_type = type;
	_hp = 100;
	_x = 0;
	_y = 0;
	_state = IDLE;
	_id = id;
	std::cout << "registered unit " << id << std::endl;
}

void	Unit::move(int dir)
{
	switch (dir)
	{
		case NORTH:
			_y += 1;
			break ;
		case SOUTH:
			_y -= 1;
			break ;
		case EAST:
			_x += 1;
			break ;
		case WEST:
			_x -= 1;
			break ;
		default:
			break ;
	}
}

void	UnitManager::processCommands(void)
{
	for (std::map<std::string, std::pair<int, int>>::iterator it=_commands.begin(); it != _commands.end(); ++it)
		std::cout << it->first << ":" << std::endl;
}

void	UnitManager::removeUnit(std::string id)
{
	delete _units[id];
	_units.erase(id);
}

UnitManager::UnitManager() { _unitCount = 0; }
UnitManager::~UnitManager() { std::cout << "unit manager destroyed\n"; }
void	UnitManager::addUnit(int type)
{
	_units.insert(std::make_pair(std::to_string(_unitCount),
					new Unit(type, _unitCount)));
	_unitCount++;
}
Unit*	UnitManager::getUnit(std::string id) { return _units[id]; }
void	Unit::damage(int amt) { _hp -= amt; }
void	Unit::heal(int amt) { _hp += amt; }
void	Unit::build(int type) { std::cout << "build " << type << std::endl; }
size_t	Unit::getType() { return _type; }
int		Unit::getHp() { return _hp; }
int		Unit::getX() { return _x; }
int		Unit::getY() { return _y; }
int		Unit::getState() { return _state; }
size_t	Unit::getId() { return _id; }
void	Unit::setX(int x) { _x = x; }
void	Unit::setY(int y) { _y = y; }
Unit::~Unit() { std::cout << _id << " has died\n"; }
