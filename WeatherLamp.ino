import cc.arduino.*;
import org.firmata.*;
import cc.arduino.*; //ArduinoLibrary
import processing.serial.*;

String arduinoPort = "/dev/cu.usbserial-A600afDy";
Arduino arduino;

String baseURL = "http://weather.livedoor.com/forecast/webservice/json/v1?city=";
String city = "140010";
JSONArray forecasts;
String telop;
String title;

//時間管理
float playTime;
float elapsedTime = 0;

int brightness = 10;    // how bright the LED is
int fadeAmount = 7;    // how many points to fade the LED by

int redLED = 9;
int blueLED = 10;
int greenLED = 11;

void setup(){
    //Arduinoイニシャライズ
    arduino = new Arduino(this, arduinoPort, 57600);

    //ピンの要素取得
    arduino.pinMode(redLED, Arduino.OUTPUT);
    size(200, 200);

    getInfo();
}

void draw(){
    
    //ウインドウ表示
    background(255);
    fill(0);
    
    if(elapsedTime > 86400000){
        getInfo();
        
    }

    elapsedTime = millis() - playTime;
    //情報表示
    text(title, 10, 35);
    text(telop, 10, 55);

    if(telop.contains("晴")){
        //赤色がひかる
        beautifulLighting(redLED);
    }
    
    if(telop.contains("雨")){
        //青が光る
        beautifulLighting(blueLED);
    }
    
    if(telop.contains("曇")){
        //緑が光る
        beautifulLighting(greenLED);
    }

    println("elapsedTime: "+elapsedTime);
    // println("telop: "+telop);
    // 86400000
    
}


//お天気データをゲットする
void getInfo(){

    JSONObject w = loadJSONObject(baseURL + city);
    title =  w.getString("title");
    forecasts = w.getJSONArray("forecasts");

    JSONObject f = forecasts.getJSONObject(0);
    telop = f.getString("telop");

    playTime = millis();

}

//美しく（フェードしながら）光る
void beautifulLighting(int light){
    arduino.analogWrite(light, brightness);

    // change the brightness for next time through the loop:
    brightness = brightness + fadeAmount;

    // reverse the direction of the fading at the ends of the fade:
    if (brightness <= 10 || brightness >= 255) {
        fadeAmount = -fadeAmount;
    }
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
}
