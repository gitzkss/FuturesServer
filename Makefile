server:
	g++ src/util.cpp client.cpp -o client && \
	g++ server.cpp \
	src/util.cpp src/Epoll.cpp src/InetAddress.cpp src/Socket.cpp src/Connection.cpp \
	src/Channel.cpp src/EventLoop.cpp src/Server.cpp src/Acceptor.cpp \
	src/chi_cang.cpp \
	src/utils.cpp \
	src/wei_tuo.cpp \
	src/cuo_he_server.cpp \
	src/customer_info.cpp \
	src/jiao_yi_data.cpp \
	-o server
clean:
	rm server && rm client