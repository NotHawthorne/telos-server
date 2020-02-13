#include "../../includes/server.hpp"

void	MapChunk::crawl(int x, int y, int type)
{
	int	life = 30 + (rand() % 40);
	int	curx = x;
	int	cury = y;
	int	tmp = 0;

	while (life)
	{
		tmp = rand() % 4;
		if (tmp == 0 && curx < CHUNK_SIZE)
			_tiles[cury][curx++] = type;
		else if (tmp == 1 && cury < CHUNK_SIZE)
			_tiles[cury++][curx] = type;
		else if (tmp == 2 && curx > 0)
			_tiles[cury][curx--] = type;
		else if (tmp == 3 && cury > 0)
			_tiles[cury--][curx] = type;
		else
			break ;
		life--;
	}
}

void	MapChunk::populate()
{
	int	ants;

	ants = 28 + (rand() % 5);
	if (_north)
	{
		for (int i = 0; i < CHUNK_SIZE; i++)
		{
			if (_north->getTile(i, CHUNK_SIZE - 1) != GRASS)
			{
				crawl(i, 0, _north->getTile(i, CHUNK_SIZE - 1));
				ants--;
			}
		}
	}
	if (_south)
	{
		for (int i = 0; i < CHUNK_SIZE; i++)
		{
			if (_south->getTile(i, 0) != GRASS)
			{
				crawl(i, CHUNK_SIZE - 1, _south->getTile(i, 0));
				ants--;
			}
		}
	}
	if (_east)
	{
		for (int i = 0; i < CHUNK_SIZE; i++)
		{
			if (_east->getTile(0, i) != GRASS)
			{
				crawl(CHUNK_SIZE - 1, i, _east->getTile(0, i));
				ants--;
			}
		}
	}
	if (_west)
	{
		for (int i = 0; i < CHUNK_SIZE; i++)
		{
			if (_west->getTile(CHUNK_SIZE - 1, i) != GRASS)
			{
				crawl(0, i, _west->getTile(CHUNK_SIZE - 1, i));
				ants--;
			}
		}
	}
	while (ants > 0)
	{
		crawl(rand() % CHUNK_SIZE, rand() % CHUNK_SIZE, (rand() % 3) + 1);
		ants--;
	}
}

void	MapChunk::print()
{
	std::cout << "MAP CHUNK\n";
	std::cout << "=========================================\n";
	for (int i = 0; i != CHUNK_SIZE; i++)
	{
		for (int n = 0; n != CHUNK_SIZE; n++)
			std::cout << _tiles[i][n] << " ";
		std::cout << std::endl;
	}
	std::cout << "=========================================\n";
}

MapChunk::MapChunk(int x, int y)
{
	for (int i = 0; i != CHUNK_SIZE; i++)
		for (int n = 0; n != CHUNK_SIZE; n++)
			_tiles[i][n] = 0;
	_hash = time(NULL);
	_x = x;
	_y = y;
	_north = NULL;
	_south = NULL;
	_east = NULL;
	_west = NULL;
	srand(_hash);
}

MapChunk&	MapChunk::operator=(const MapChunk &x)
{
	for (int i = 0; i != CHUNK_SIZE; i++)
		for (int n = 0; n != CHUNK_SIZE; n++)
			_tiles[i][n] = x.getTile(n, i);
	_north = x.getNorth();
	_south = x.getSouth();
	_east = x.getEast();
	_west = x.getWest();
	_x = x.getX();
	_y = x.getY();
	_hash = x.getHash();
	return (*this);
}

int				MapChunk::getTile(int x, int y) const { return (_tiles[y][x]); }
MapChunk		*MapChunk::getNorth() const { return (_north); }
MapChunk		*MapChunk::getSouth() const { return (_south); }
MapChunk		*MapChunk::getEast() const { return (_east); }
MapChunk		*MapChunk::getWest() const { return (_west); }
int				MapChunk::getX() const { return (_x); }
int				MapChunk::getY() const { return (_y); }
unsigned long	MapChunk::getHash() const { return (_hash); }
void			MapChunk::setNorth(MapChunk *x) { _north = x; }
void			MapChunk::setSouth(MapChunk *x) { _south = x; }
void			MapChunk::setEast(MapChunk *x) { _east = x; }
void			MapChunk::setWest(MapChunk *x) { _west = x; }
MapChunk::MapChunk(const MapChunk &x) { *this = x; }
MapChunk::~MapChunk() { return ; }
