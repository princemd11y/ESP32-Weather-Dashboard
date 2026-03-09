#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WebServer.h>
#include <String.h>

const char* ssid = "***********";
const char* password = "***********";
const char* apiKey = "********************************";
WebServer monserver(80);
void accueil();
void meteo();

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pinMode(2, OUTPUT);
  Serial.print("Connexion au Wifi :");
  Serial.println(ssid);
  while (WiFi.status()!=WL_CONNECTED) // Tant que l'ESP32 n'est pas Well connected
  {
    Serial.println(".");
    delay(500);
    digitalWrite(2, !digitalRead(2));
  }
  Serial.println("Connecté au Wifi !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());

  monserver.on("/", accueil);           // Page d'accueil
  monserver.on("/traitement", meteo);
  
  monserver.begin();
}

void accueil() {
  monserver.send(200, "text/html", R"extr(
    <!DOCTYPE html>
    <html>
    <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>Station Météo ESP32</title>
      <style>
        * {
          margin: 0;
          padding: 0;
          box-sizing: border-box;
        }
        
        body { 
          font-family: 'Segoe UI', Arial, sans-serif;
          min-height: 100vh;
          background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
          color: white;
          padding: 20px;
          display: flex;
          flex-direction: column;
          align-items: center;
          justify-content: center;
        }
        
        .container {
          width: 100%;
          max-width: 500px;
          background: rgba(255, 255, 255, 0.1);
          backdrop-filter: blur(10px);
          border-radius: 20px;
          padding: 30px;
          box-shadow: 0 8px 32px rgba(0, 0, 0, 0.3);
        }
        
        h1 {
          font-size: clamp(24px, 5vw, 32px);
          margin-bottom: 30px;
          text-align: center;
        }
        
        label {
          font-size: clamp(16px, 3.5vw, 18px);
          display: block;
          margin-bottom: 15px;
          font-weight: 500;
        }
        
        select {
          width: 100%;
          padding: 15px;
          font-size: clamp(14px, 3vw, 16px);
          border: none;
          border-radius: 10px;
          background: white;
          color: #333;
          cursor: pointer;
          margin-bottom: 20px;
        }
        
        input[type="submit"] {
          width: 100%;
          padding: 15px;
          font-size: clamp(16px, 3.5vw, 18px);
          background: #4CAF50;
          color: white;
          border: none;
          border-radius: 10px;
          cursor: pointer;
          font-weight: bold;
          transition: background 0.3s;
        }
        
        input[type="submit"]:hover {
          background: #45a049;
        }
        
        input[type="submit"]:active {
          transform: scale(0.98);
        }
      </style>
    </head>
    <body>
      <div class="container">
        <h1>🌤️ Station Météo</h1>
        <form action="/traitement" method="GET">
          <label>Choisis une ville :</label>
          
          <select name="choixVille">
            <option value="Paris,fr">🗼 Paris</option>
            <option value="Marseille,fr">⛵ Marseille</option>
            <option value="Lyon,fr">🦁 Lyon</option>
            <option value="Toulouse,fr">✈️ Toulouse</option>
            <option value="Nice,fr">🏖️ Nice</option>
            <option value="Nantes,fr">🏰 Nantes</option>
            <option value="Strasbourg,fr">🏛️ Strasbourg</option>
          </select>
          
          <input type="submit" value="Voir la météo ☀️">
        </form>
      </div>
    </body>
    </html>
  )extr");
}

void meteo() {
   String ville = monserver.arg("choixVille");
  if (WiFi.status()==WL_CONNECTED)
  {
    HTTPClient http;
    String url = "http://api.openweathermap.org/data/2.5/weather?q=" + ville + "&APPID=" + String(apiKey) + "&units=metric";
    http.begin(url);
    int requete = http.GET();
    if(requete > 0)
    {
      String contenu = http.getString();
      JsonDocument Mondoc;
      DeserializationError error = deserializeJson(Mondoc, contenu);
      if (error) {
        Serial.print("Erreur de désérialisation JSON : ");
      } else {
        String name = Mondoc["name"];
        name.replace("Arrondissement de ", "");
        float temp = Mondoc["main"]["temp"];
        float humidity = Mondoc["main"]["humidity"];
        /*
        Serial.print("Ville : ");
        Serial.println(name);
        Serial.print("Température : ");
        Serial.println(temp);
        Serial.print("Humidité : ");
        Serial.println(humidity);       
        */
        Serial.print(name);   
        Serial.print(",");    
        Serial.print(temp);  
        Serial.print(",");    
        Serial.println(humidity);

      String pageResultat = R"extr(
        <!DOCTYPE html>
        <html>
        <head>
          <meta charset="UTF-8">
          <meta name="viewport" content="width=device-width, initial-scale=1.0">
          <title>Météo )extr";
              pageResultat += name;
              pageResultat += R"extr(</title>
          <style>
            * {
              margin: 0;
              padding: 0;
              box-sizing: border-box;
            }
            
            body {
              font-family: 'Segoe UI', Arial, sans-serif;
              min-height: 100vh;
              background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
              color: white;
              padding: 20px;
              display: flex;
              flex-direction: column;
              align-items: center;
              justify-content: center;
            }
            
            .container {
              width: 100%;
              max-width: 500px;
              background: rgba(255, 255, 255, 0.1);
              backdrop-filter: blur(10px);
              border-radius: 20px;
              padding: 30px;
              box-shadow: 0 8px 32px rgba(0, 0, 0, 0.3);
              text-align: center;
            }
            
            h1 {
              font-size: clamp(20px, 4vw, 28px);
              margin-bottom: 10px;
            }
            
            .city {
              font-size: clamp(28px, 6vw, 40px);
              font-weight: bold;
              margin-bottom: 30px;
              color: #FFD700;
            }
            
            .meteo-box {
              background: rgba(255, 255, 255, 0.15);
              border-radius: 15px;
              padding: 25px;
              margin: 20px 0;
            }
            
            .temp {
              font-size: clamp(48px, 10vw, 72px);
              color: #FFD700;
              font-weight: bold;
              margin: 10px 0;
            }
            
            .humidity {
              font-size: clamp(20px, 4vw, 28px);
              color: #87CEEB;
              margin: 10px 0;
            }
            
            .btn {
              display: inline-block;
              margin-top: 25px;
              padding: 15px 30px;
              background: #4CAF50;
              color: white;
              text-decoration: none;
              border-radius: 10px;
              font-weight: bold;
              font-size: clamp(14px, 3vw, 16px);
              transition: background 0.3s;
            }
            
            .btn:hover {
              background: #45a049;
            }
            
            .btn:active {
              transform: scale(0.98);
            }
          </style>
        </head>
        <body>
          <div class="container">
            <h1>🌤️ Météo en direct</h1>
            <div class="city">📍 )extr";
              pageResultat += name;
              pageResultat += R"extr(</div>
            
            <div class="meteo-box">
              <div class="temp">🌡️ )extr";
              pageResultat += String(temp, 1);
              pageResultat += R"extr( °C</div>
              <div class="humidity">💧 )extr";
              pageResultat += String(humidity, 0);
              pageResultat += R"extr( %</div>
            </div>
            
            <a href="/" class="btn">← Retour au menu</a>
          </div>
        </body>
        </html>
        )extr";
        monserver.send(200, "text/html", pageResultat);
      }
      http.end();
    }
    else
    {
      Serial.print("Erreur de requete : ");
      Serial.println(requete);
      http.end();
    }
  }
  else
  {
    Serial.println("Erreur de connexion au Wifi");
  }
}


void loop() {
  monserver.handleClient();
}
