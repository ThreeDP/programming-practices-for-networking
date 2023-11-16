#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

typedef struct addrinfo t_addrinfo;

/*
$ showip www.example.net
	Endereço IP para www.example.net:
		IPv4: 192.0.2.88
$ showip ipv6.example.com
	Endereço IP para ipv6.example.com:
		IPv4: 192.0.2.101
		IPv6: 2001:db8:8c00:22::171
*/
int main(int ac, char **av) {
	t_addrinfo		*p;
	t_addrinfo 		*res;
	t_addrinfo 		hints;
	int 			status;
	char			ipstr[INET6_ADDRSTRLEN];

	if (ac != 2) {
		std::cout << "uso: show ip hostname\n" << std::endl;
		return 1;
	}

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if ((status = getaddrinfo(av[1], NULL, &hints, &res)) != 0) {
		std::cout << "getaddrinfo: " << gai_strerror(status) << std::endl;
		return 2;
	}

	std::cout << "Endereço IP para " << av[1] << std::endl;

	for (p = res; p; p = p->ai_next) {
		void 		*addr;
		std::string ipver;

		if (p->ai_family == AF_INET) { // IPv4
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		} else {
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}
		inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
		std::cout << "\t" << ipver << "\t" << ipstr << std::endl;
	}
	freeaddrinfo(res);
	return 0;
}
