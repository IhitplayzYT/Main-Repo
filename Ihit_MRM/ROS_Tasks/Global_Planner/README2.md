1.To navigate rover from one latitude/longitude to another in a world with no obstacles 
2.The twist_pub is publishing to the topic cmd_vel which is responsible for sending movement msgs to rover.
The gps_sub is subscribed to the /gps topic so that it can obtain it's current gps latitude and longitude 
The method works by moving in the x direction the rotation 90 degree and moving the y disstance to raech the coordinates...as close as possible
3.![image](https://github.com/user-attachments/assets/145979df-1f40-4ff8-acc9-38414af822d2)
4.Youtube Link : https://youtu.be/3py35EbUGPk
