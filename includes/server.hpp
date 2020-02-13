#ifndef SERVER_HPP
# define SERVER_HPP

# include <unistd.h>
# include <sys/socket.h>
# include <stdlib.h>
# include <string.h>
# include <iostream>
# include <netinet/in.h>
# include <time.h>
# include <pthread.h>
# include <sstream>
# include <vector>
# include <map>
# include <fcntl.h>
# include <errno.h>
# include "user.hpp"
# include "unit.hpp"

# define CHUNK_SIZE 64

enum PacketTypes {
	SYSTEM,
	USER
};

enum UserPackets {
	MOVE
};

enum SystemPackets {
	GAME_STATE,
	REGISTER_UNIT,
	UPDATE_UNIT
};

enum {
	GRASS,
	MOUNTAIN,
	WATER,
	LAVA
};

class MapChunk	{
private:
	int 			_tiles[CHUNK_SIZE][CHUNK_SIZE];
	MapChunk		*_north;
	MapChunk		*_south;
	MapChunk		*_east;
	MapChunk		*_west;
	int				_x;
	int				_y;
	unsigned long	_hash;
public:
	MapChunk(int x = 0, int y = 0);
	~MapChunk();
	MapChunk(const MapChunk &x);
	MapChunk&	operator=(const MapChunk &x);

	void			populate();
	void			crawl(int x, int y, int type);
	void			print();
	int				getTile(int x, int y) const;
	MapChunk		*getNorth() const;
	MapChunk		*getSouth() const;
	MapChunk		*getEast() const;
	MapChunk		*getWest() const;
	int				getX() const;
	int				getY() const;
	unsigned long	getHash() const;
	void			setNorth(MapChunk *x);
	void			setSouth(MapChunk *x);
	void			setEast(MapChunk *x);
	void			setWest(MapChunk *x);
};

#endif
