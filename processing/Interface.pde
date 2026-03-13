/*
 * Affichage temps reel des donnees meteo
 * Recoit les donnees CSV depuis l'ESP32 via port serie
 */
//Importation de la bibliotheque
import processing.serial.*;

Serial monPort; 

// Variables globales pour stocker les donnees
String ville = "--";
String temperature = "--";
String humidite = "--";
String derniereMAJ = "--";

void setup() {
  pixelDensity(1);
  size(800, 500);

  //Attribution du port (a verifier dans le gestionnaire de peripheriques)
  monPort = new Serial(this, "COM6", 115200);
  monPort.bufferUntil('\n');
}

void draw() {
  background(40);
  
  // === EN-TÊTE ===
  fill(255);
  textSize(32);
  textAlign(CENTER);
  text("STATION METEO ESP32", width/2, 50);
  
  // === CARTE VILLE ===
  //Rectangle
  fill(255, 255, 255, 30);
  stroke(255, 255, 255, 80);
  strokeWeight(2);
  rect(width/2 - 180, 90, 360, 70, 10);

  //Texte
  fill(100, 200, 255);
  textSize(28);
  noStroke();
  text(ville, width/2, 130);
  
  // === CARTE TEMPÉRATURE ===
  //Rectangle
  fill(255, 100, 100, 30);
  stroke(255, 100, 100, 100);
  strokeWeight(2);
  rect(100, 200, 250, 180, 10);

  //Texte
  fill(255, 150, 150);
  textSize(20);
  noStroke();
  text("TEMPERATURE", 225, 240);
  
  fill(255, 200, 200);
  textSize(60);
  text(temperature + " C", 225, 310);
  
  // === CARTE HUMIDITÉ ===
  //Rectangle
  fill(100, 150, 255, 30);
  stroke(100, 150, 255, 100);
  strokeWeight(2);
  rect(450, 200, 250, 180, 10);

  //Texte
  fill(150, 200, 255);
  textSize(20);
  noStroke();
  text("HUMIDITE", 575, 240);
  
  fill(200, 220, 255);
  textSize(60);
  text(humidite + " %", 575, 310);
  
  // === Infos sup ===
  //Texte
  fill(255, 255, 255, 150);
  textSize(14);
  textAlign(LEFT);
  text("Derniere MAJ : " + derniereMAJ, 30, height - 20);
  
  textAlign(RIGHT);
  text("Port : COM6 | 115200 baud", width - 30, height - 20);
}


void serialEvent(Serial monPort) {
  //Lecture du port
  String data = monPort.readStringUntil('\n');

  //Verifier si on a bien reçu un fichier
  if (data != null) {
    data = trim(data);
    
    // Verifier que les donnees contiennent bien des virgules (format CSV)
    if (data.contains(",")) {
      
      String[] valeurs = split(data, ',');

      //Verifier si l'on a bien reçoit bien les 3 infos Ville, Temp, Hum
      if (valeurs.length == 3) {
        ville = valeurs[0];
        temperature = valeurs[1];
        humidite = valeurs[2];
        
        // Recuperation de l'heure de recuperation
        derniereMAJ = nf(hour(), 2) + ":" + nf(minute(), 2) + ":" + nf(second(), 2);
        
        // Affichage test sur la console
        println("Recu : " + ville + " | " + temperature + "C | " + humidite + "%");
      }
    }
  }
}
