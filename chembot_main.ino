#include "HX711.h"                                          //Incloem la llibreria.
HX711 pesA(A1, A0);                                         //Declarem els pins de les 
HX711 pesB(A3, A2);                                         //cel·les A i B.
float taraA = pesA.read_average(100);                       //Declarem una variable de tara
float taraB = pesB.read_average(100);                       //per aconseguir el pes més tard.

const int bombaA = 7;                                       //Declarem els pins de 
const int bombaB = 8;                                       //les bombes A i B. 

float lectura_pH;                                           //Creem una variables per a llegir valors de pH.
float mitja_pH;                                             //Creem una variable per fer la mitja de les lectures de pH. 
float pH;                                                   //Creem una variables per determinar el valor final del pH. 

unsigned long temps;                                        //Creem una variable de temps per despré saber quant de temps porta el programa funcionant.

float valorA_0;                                             //Creem una variable float (nombres amb molts decimals) per al pes A inicial.
float valorB_0;                                             //Creem una variable float (nombres amb molts decimals) per al pes B inicial.
float valorA;                                               //Creem una variable float (nombres amb molts decimals) per al pes A. 
float valorB;                                               //Creem una variable float (nombres amb molts decimals) per al pes B.

void setup() {                                              //Inici del setup.
  temps = millis();                                         //Fem que llegeixi el temps en milisegons.
  mitja_pH = 0;                                             //Donem un valor inicial a la mitja del pH.
  
  pinMode(bombaA, OUTPUT);                                  //Declarem les bombes
  pinMode(bombaB, OUTPUT);                                  //com a sortides.
  
  for (int j=0; j<30; j++){                                 //Creem un loop de for amb les tres condicions.
    lectura_pH = analogRead(A4);                            //La variable lectura és igual al valor que llegeixi del pin analògic A4.
    mitja_pH = mitja_pH + lectura_pH/30.0;                  //Aconseguim una mitja de 30 lectures. 
    }
  pH = 0.0177 * mitja_pH - 0.006;                           //Aconseguim el valor del pH amb l'equació. 
  
  Serial.begin(9600);                                       //Inicialitzem el monitor sèrie a 9600 baud.
  Serial.println("Carregar mostres!");                      //Imprimim l'ordre de carregar les mostres a les balançes al monitor sèrie.
  delay(12000);                                             //Decalatge de 12 segons, per assegurar-nos de que tenim temps a posar les mostres.

  Serial.print("Mitja A: ");                                //Imprimeix la mitja de.
  Serial.print(pesA.read_average(30));                      //30 mesures de la cel·la A.
  Serial.print(" || ");
  Serial.print("Mitja B: ");                                //Imprimeix la mitja de. 
  Serial.println(pesB.read_average(30));                    //30 mesures de la cel·la B.

  delay(200);                                               //Decalatge de 0.2 segons.

  valorA_0 = (pesA.read_average(30)-taraA) * 1.0614e-03;    //Aconseguim valors en grams de les cel·les
  valorB_0 = (pesB.read_average(30)-taraB) * 9.9654e-04;    //A i B mitjançant una equació.

  Serial.print("Temps transcorregut(s): ");                 //Imprimeix els segons transcorreguts des de l'inici del programa,
  Serial.println(temps/1000);                               //escrivim temps/1000 perquè llegeix milisegons i volem que ens doni els segons.
  Serial.print(" || ");
  Serial.print("Grams inicials d'A: ");                     //Imprimeix els 
  Serial.print(valorA_0);                                   //grams d'A.
  Serial.print(" || ");
  Serial.print("Grams inicials de B: ");                    //Imprimeix els
  Serial.print(valorB_0);                                   //grams de B.
  Serial.print(" || ");
  Serial.print("Valor del pH inicial: ");                   //Imprimeix el valor
  Serial.print(pH);                                         //de pH inicial.
                                                            
                                                            
}                                                           
                                                            
void loop() {                                               //Inici del loop principal, que s'anirà repetint constantment.
  temps = millis();                                         //El temps el llegirà en milisegons.

  float valorA_f = valorA_0 - 100.0;                        //Resta el valor de dissolució A a extreure en grams.
  valorA = (pesA.read_average(30)-taraA) * 1.0614e-03;      //Mesura el pes valorA.
  //Serial.print("Pes balança A: ");                          //Imprimeix el (no útil --> posat com a comentari).
  //Serial.println(valorA);                                   //valor del pes d'A.
  
  if (valorA > valorA_f) {                                  //Iniciem un if amb la seva condició.
    mitja_pH = 0;                                           //Donem valor inicial a la mitja del pH.
    for(int j = 0; j < 30; j++){                            //Iniciem un loop de tipus for per calcular el pH.
      lectura_pH = analogRead(A4);                          //La variable lectura_pH és igual a la lectura del pin analògic A4.
      mitja_pH = mitja_pH + lectura_pH / 30.0;              //Calcula el valor mitjà del pH amb 30 lectures.
      }
    pH = 0.0177 * mitja_pH - 0.006;                         //Utilitzant l'equaciò de calibrat prèviament trobada, es calcula el valor real del pH. 
    while(pH < 9) {                                         //S'inicia un loop de tipus while, amb la condició de que el pH s'ha de mantenir inferior a 9.
      digitalWrite(bombaB, HIGH);                           //Posem la bomba B en funcionament 
      delay(300);                                           //durant 0.3 segons.
      digitalWrite(bombaB, LOW);                            //Parem la bomba B.
      valorB = (pesB.read_average(30)-taraB) * 9.9654e-04;  //Calculem el valor del pes B. 
      //Serial.print("Pes balança B: ");                      //Imprimim el valor (no útil --> posat com a comentari).
      //Serial.println(valorB);                               //del pes B.
      delay(4000);                                          //Decalatge de 4 segons. 
      mitja_pH = 0;                                         //Li donem el valor de 0 a la mitja del pH.
      for(int k = 0; k < 30; k++) {                         //Iniciem un altre loop de tipus for per calcuar el pH.
        lectura_pH = analogRead(A4);                        //La variable lectura_pH és igual a la lectura del pin analògic A4.
        mitja_pH = mitja_pH + lectura_pH / 30.0;            //Calcula el valor mitjà del pH amb 30 lectures.
        }
      pH = 0.0177 * mitja_pH - 0.006;                       //Utilitzant l'equaciò de calibrat prèviament trobada, es calcula el valor real del pH. 
      }
      digitalWrite(bombaA, HIGH);                           //Posem la bomba A en funcionament
      delay(400);                                           //durant 0.4 segons.
      digitalWrite(bombaA, LOW);                            //Parem la bomba A.
      valorA = (pesA.read_average(30)-taraA) * 1.0614e-03;  //Calcula el valor del pes A mitjançant el pendent de la balança A.
      valorB = (pesB.read_average(30)-taraB) * 9.9654e-04;  //Calcula el valor del pes B mitjançant el pendent de la balança B. 
      Serial.print("Temps (s): ");                          //Imprimeix el valor   
      Serial.print(temps/1000);                             //del temps en segons.
      Serial.print(" || ");                                 
      Serial.print("Pes balança A: ");                      //Imprimeix el
      Serial.print(valorA);                                 //valor del pes A.
      Serial.print(" || ");                                 
      Serial.print("Pes balança B: ");                      //Imprimeix el 
      Serial.print(valorB);                                 //valor del pes B.
      Serial.print(" || ");                                 
      Serial.print("Valor del pH: ");                       //Imprimeix el 
      Serial.println(pH);                                   //valor del pH.
    } else {                                                //Iniciem la condició else en contraposició a la condició de l'if inicial.
      digitalWrite(bombaA, LOW);                            //Parem la bomba A.
      mitja_pH = 0;                                         //Li donem valor 0 a la mitja del pH.
      while(pH < 9) {                                       //S'iniia un loop de tipus while, amb la condició que s'executi quan el pH sigui inferior a 9. 
        digitalWrite(bombaB, HIGH);                         //Posem la bomba B en funcionament
        delay(300);                                         //durant 0.3 segons.
        digitalWrite(bombaB, LOW);                          //Parem la bomba B.
        for(int q = 0; q < 30; q++) {                       //Iniciem un loop de tipus for amb les tres condicions.
        lectura_pH = analogRead(A4);                        //La variable lectura_pH és igual a la lectura de la sonda de pH.
        mitja_pH = mitja_pH + lectura_pH / 30.0;            //Calcula el valor mitjà del pH amb 30 lectures.
        }
      pH = 0.0177 * mitja_pH - 0.006;                       //Utilitzant l'equaciò de calibrat prèviament trobada, es calcula el valor real del pH.
      }
      if(pH >= 9) {                                         //Iniciem un if amb la seva condició.
        digitalWrite(bombaA, LOW);                          //Parem la bomba A.
        digitalWrite(bombaB, LOW);                          //Parem la bomba B.
        Serial.println("Fí del procés.");                   //Imprimim la frase "Fí del procés".
        delay(1500);                                        //decalatge d'1.5 segons.
        }
      }
  

}
