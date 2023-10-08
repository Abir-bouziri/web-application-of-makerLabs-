#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
//bibliothéque SPIFFS
#include "FS.h"
const char* ssid = "**************"; //ssid of your wifi
const char* password = "*****************";
//les moteurs pins 
int ENA = D7;
int in1 = D1;
int in2 = D2;
int ENB = D6;
int in3 = D3;
int in4 = D4;
ESP8266WebServer server(80);
 int y=100;
 String state="s";
void setup() {
   // Initialize Serial communication
  Serial.begin(115200);
  //setups moteur pins
  pinMode(ENA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(in3, LOW);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  //get the ip @ adress of the esp8266
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
  //activate spiffs
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");

    return;
  }
  else {Serial.println("Sucess");}
  //start server
  server.begin();
  //configuration of url 
  server.on("/",HTTP_GET,login);
  server.on("/login", HTTP_GET,handleRoot);
  // Define a route to handle the form submission
  server.on("/style", HTTP_GET,css);
  server.on("/csslogin",HTTP_GET,csslogin);
  server.on("/login/moteurstate", HTTP_GET,moteur_control);
  server.on("/login/moteur", HTTP_GET,vitessemoteur);
  server.on("/photo",photo);
  
}
void loop() {
  server.handleClient();
}
//png file 
void photo(){
  File imageFile = SPIFFS.open("/maker.png", "r"); // Open the image file
  if (!imageFile) {
    Serial.println("Failed to open image file");
    server.send(404, "text/plain", "Image not found");
    return;
  }

  server.streamFile(imageFile, "image/png"); // Send the image as a response
  imageFile.close();
  }
//css file for login page
void csslogin(){
  File styles = SPIFFS.open("/csslogin.css", "r");

    if (!styles) {
      Serial.println("Failed to open file css");
      return server.send(404, "text/plain", "File not found");
    }
     server.send(200, "text/css", styles.readString());   
    styles.close();}
// read login page 
void login(){
  File login = SPIFFS.open("/index.html", "r"); // Replace "your_html_file.html" with your file's path
    if (!login) {
      Serial.println("Failed to open file html");
      return server.send(404, "text/plain", "File not found");
    }
    // server.sendHeader("Content-Encoding", "gzip");
    server.send(200, "text/html", login.readString());    
    login.close();
    }
//fonction to read the css file 
void css(){
  File styles = SPIFFS.open("/styles.css", "r");

    if (!styles) {
      Serial.println("Failed to open file css");
      return server.send(404, "text/plain", "File not found");
    }
     server.send(200, "text/css", styles.readString());   
    styles.close();
}
// fonction to read the html file with css file 
void handleRoot() {
File file = SPIFFS.open("/styles.html", "r"); // Replace "your_html_file.html" with your file's path
    if (!file) {
      Serial.println("Failed to open file html");
      return server.send(404, "text/plain", "File not found");
    }
    // server.sendHeader("Content-Encoding", "gzip");
    server.send(200, "text/html", file.readString());    
    file.close();
  }
//fonction to control the moteur 
void moteur_control() 
{
 String state = "S";
 String act_state = server.arg("state");
 if(act_state == "f")
 {
  Serial.println("forward"); 
  forward(); delay(250); stopt();
 }
 if(act_state == "b")
 {
  Serial.println("back");
  backward(); delay(250); stopt();
 }
 if(act_state == "r")
 {
  Serial.println("right");
  right(); delay(250); stopt();
 }
 if(act_state == "l")
 {
  Serial.println("left");
  left(); delay(250); stopt();
 }
 else
 {
  Serial.println("stop"); stopt();
 }
 
 server.send(200, "text/plane", act_state);
}
//fonction to vitesse 
void vitessemoteur(){
 String v = server.arg("vitesse");
 if(v== "plus")
 {
  y=y+5;
  Serial.println(y);
 }
  if(v== "-")
 {
  y=y-5;
  Serial.println(y);
 }
 server.send(200, "text/plane",String(y));
 } 
 
void stopt(void) {
  analogWrite(ENA, 0); //speed 0 - 255
  analogWrite(ENB, 0); //speed 0 - 255
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
void backward(void) {
  analogWrite(ENA, y); //speed 0 - 255
  analogWrite(ENB, y); //speed 0 - 255
  digitalWrite(in1, false);
  digitalWrite(in2, true);
  digitalWrite(in3, false);
  digitalWrite(in4, true);
}

void forward(void) {
  analogWrite(ENA, y); //speed 0 - 255
  analogWrite(ENB, y); //speed 0 - 255
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void right(void) {
  analogWrite(ENA, y); //speed 0 - 255
  analogWrite(ENB, y); //speed 0 - 255
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void left(void) {
  analogWrite(ENA, y); //speed 0 - 255
  analogWrite(ENB, y); //speed 0 - 255
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
