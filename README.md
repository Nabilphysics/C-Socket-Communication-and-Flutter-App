# Socket-Communication-and-Flutter-App
 C++ BOOST ASIO SERVER and CLIENT. Also, a Flutter Client App to send and receive data.
 
This project can be done in various way. But, I have used C++ as much as possible. Basically, I have used here boost asio C++ library for Socket communication in client server mode. I tried to make it like a real project. In this project, first we need to start the C++ based server. The hardware client can connect with this server using IP and Port number. Then mobile app that I have developed(using Flutter) can also connect with this server. Now, if hardware client send any data in JSON format, the mobile app can receive and also mobile app can send data to hardware client. Server can trigger notification as well as notification details to app client. App client can request image from Hardware Client. If hardware client receive image sending request then it takes image using C++ camera driver and send it to client mobile App. Mobile app can run in both iOS and Android. 

Code Link for Easy Lookup: 
Flutter App Link: https://github.com/Nabilphysics/Flutter_TCP_Socket_Connection
</br>C++ TCP Client: https://github.com/Nabilphysics/C-Socket-Communication-and-Flutter-App/blob/main/Client_Hardware/Client_hardware_code/chat_client.cpp
</br>C++ Server(100% from BOOST ASIO): https://github.com/Nabilphysics/C-Socket-Communication-and-Flutter-App/tree/main/Server/Server_code_c%2B%2B
</br>Hardware Side Skeleton Code and Driver: https://github.com/Nabilphysics/C-Socket-Communication-and-Flutter-App/tree/main/hardware%20driver%20code 
