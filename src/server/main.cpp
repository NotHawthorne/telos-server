#define PORT 4242

#include "../../includes/server.hpp"

UserManager	g_manager;
size_t		g_alive_units = 0;

std::string	handshake(std::string packet)
{
	std::vector<std::string> strings;
	std::istringstream f(packet);
	std::string	s;
	while (getline(f, s, '|'))
		strings.push_back(s);
	return (strings[2]);
}

std::string	structure_packet(int type, int subtype, std::string data)
{
	std::string ret;

	ret += std::to_string(type);
	ret += "|";
	ret += std::to_string(subtype);
	ret += "|";
	ret += data;
	ret += "]";
	return (ret);
}

std::string	handle_move(std::vector<std::string> packet, User *user)
{
	Unit	*u;
	std::string	response("");

	std::cout << "responding to handle move\n";
	u = user->unitMan.getUnit(packet[2]);
	if (u)
		u->move(std::stoi(packet[3]));
	else
		std::cout << "move called on invalid unit\n";
	response += std::to_string(u->getId());
	response += "|";
	response += std::to_string(u->getX());
	response += "|";
	response += std::to_string(u->getY());
	std::cout << "response formulated (" << response << ")\n";
	return (response);
}

std::string	packet_handler(std::string packet, User *user)
{
	std::vector<std::string> substrs;
	std::vector<std::string> packets;
	std::istringstream g(packet);
	std::string s;
	std::string p;
	std::string	response("");

	std::cout << "here\n";
	while (getline(g, p, ']'))
		packets.push_back(p);
	for (int i = 0; i != packets.size(); i++)
	{
		std::istringstream f(packets[i]);
		substrs.clear();
		std::cout << "packet: " << packets[i] << std::endl;
		while (getline(f, s, '|'))
			substrs.push_back(s);
		if (substrs[0][0] == '1')
		{
			std::cout << "there\n" << substrs.size() << std::endl;
			if (substrs[1][0] == '0');
			{
				response += structure_packet(0, 2, handle_move(substrs, user));
				std::cout << response << std::endl;
			}
		}
	}
	return (response);
}

void		send_packet(std::string packet, int sockfd)
{
	std::cout << "sending " << packet.c_str() << "[" << packet.length() << "]" << std::endl;
	send(sockfd, packet.c_str(), packet.length(), 0);
}

void		send_unit_packet(Unit *unit, int sockfd)
{
	std::string	out;

	out += std::to_string(unit->getId());
	out += "|";
	out += std::to_string(unit->getHp());
	out += "|";
	out += std::to_string(unit->getX());
	out += "|";
	out += std::to_string(unit->getY());
	out += "|";
	out += std::to_string(unit->getType());
	out = structure_packet(SYSTEM, REGISTER_UNIT, out);
	send_packet(out, sockfd);
}

void	*client_listener(void *s)
{
	char	buffer[1024] = { 0 };
	int		bytes = 0;
	int		sockfd = *(int*)s;
	int		auth = 0;
	std::string	key;
	User	*user;

	user = NULL;
	std::cout << "started listener for " << sockfd << std::endl;
	fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK);
	for (;;)
	{
		bytes = read(sockfd, buffer, 1024);
		if (bytes == 0)
			break ;
		if (bytes > 0 && !auth && buffer[0] == '0')
		{
			key = handshake(std::string(buffer, buffer + bytes));
			g_manager.addUser(key);
			user = g_manager.getUser(key);
			auth++;
			std::cout << "user " << key << " registered\n";
			user->unitMan.addUnit(BUILDER);
			user->unitMan.addUnit(BUILDER);
			user->unitMan.addUnit(BUILDER);
			user->unitMan.getUnit("1")->setX(50);
			user->unitMan.getUnit("1")->setY(50);
			user->unitMan.getUnit("2")->setX(100);
			user->unitMan.getUnit("2")->setY(100);
			send_unit_packet(user->unitMan.getUnit("0"), sockfd);
			send_unit_packet(user->unitMan.getUnit("1"), sockfd);
			send_unit_packet(user->unitMan.getUnit("2"), sockfd);
		}
		else if (bytes > 0)
		{
			std::cout << "RECIEVED" << bytes << buffer << std::endl;
			std::string response = packet_handler(std::string(buffer, buffer + bytes), user);
			if (response.length())
				send(sockfd, response.c_str(), response.length(), 0);
		}
		for (int i = 0; buffer[i] && i < 1024; i++)
			buffer[i] = 0;
		user->unitMan.processCommands();
	}
	g_manager.removeUser(key);
	std::cout << "connection " << sockfd << " closed\n";
	close(sockfd);
	return (NULL);
}

void	*main_listener(void *p)
{
	int	server_fd, new_socket, valread;
	struct sockaddr_in addr;
	int opt = 1;
	int addrlen = sizeof(addr);
	char buffer[1024] = {0};
	pthread_t	t2;
	int	port = *(int*)p;

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		std::cout << "socket err\n";
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &opt, sizeof(opt)))
		std::cout << "opt err\n";
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		std::cout << "bind fail\n";
	if (listen(server_fd, 3) < 0)
		std::cout << "listen error\n";
	else
		std::cout << "listening on port " << port << std::endl;
	for (;;)
	{
		if ((new_socket = accept(server_fd, (struct sockaddr *)&addr, (socklen_t*)&addrlen)) < 0)
			std::cout << "error accepting\n";
		else
		{
			std::cout << "accepted\n";
			pthread_create(&t2, NULL, client_listener, &new_socket);
		}
	}
	return (NULL);
}

int		main(void)
{
	MapChunk	*testchunk = new MapChunk();
	MapChunk	*chunk2 = new MapChunk();
	pthread_t	t1;
	int			p = 4242;


	testchunk->populate();
	testchunk->setSouth(chunk2);
	chunk2->setNorth(testchunk);
	chunk2->populate();

	testchunk->print();
	chunk2->print();

	pthread_create(&t1, NULL, main_listener, &p);
	for (;;)
	{
		//...
	}
	return 0;
}
