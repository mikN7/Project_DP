/****************************************
*   Wzorzec Projektowy Builder          *
*   (budowniczy)                        *  
*   www.algorytm.org                    *
*   Opracowal Dworak Kamil              *
*****************************************/
 
#include <iostream>
#include <string>
using namespace std;
 
 
class ZestawKomputerowy {
 
private:
string monitor;
string procesor;
string grafika;
string ram;
string hdd;
public:
void setMonitor(string m){
monitor = m ;
}
 
void setProcesor(string p){
procesor = p;
}
 
void setGrafika(string g){
grafika = g;
}
 
void setRam(string r){
ram = r;
}
 
void setHdd(string h){
hdd = h;
}
 
void show(){
if(monitor!="") cout<< "Monitor = " << monitor << endl;
if(procesor!="") cout << "Procesor = " << procesor << endl;
if(grafika!="") cout << "Grafika = " << grafika << endl;
if(ram!="") cout << "RAM = " << ram << endl;
if(hdd!="") cout << "HDD = " << hdd << endl;
}
};
 
/* nasz glowny interdace */
class Builder {
protected:
ZestawKomputerowy* zestawKomputerowy;
 
public:
void newZestaw(){
zestawKomputerowy = new ZestawKomputerowy();
}
 
ZestawKomputerowy getZestaw(){
return* zestawKomputerowy;
}
 
virtual void buildMonitor()=0;
virtual void buildProcesor()=0;
virtual void buildGrafika()=0;
virtual void buildRam()=0;
virtual void buildHdd()=0;
};
 
class ZestawXT001:public Builder {
public :
 
ZestawXT001():Builder(){
}
 
void buildMonitor(){
zestawKomputerowy->setMonitor("Benq 19");
}
 
void buildProcesor(){
zestawKomputerowy->setProcesor("amd");
}
 
void buildGrafika(){
zestawKomputerowy->setGrafika("ATI");
}
 
void buildRam(){
zestawKomputerowy->setRam("DDR3");
}
 
void buildHdd(){
 
int t;
while(true){
cout << "Dysk do wyboru: (1) Samsung, (2) Segate, (3) Caviar"<<endl;
cin >> t;
if(t>0 && t<4) break;
}
 
string wynik;
if(t==1) wynik = "Samsung";
else if(t==2) wynik = "Segate";
else if(t==3) wynik = "Caviar";
 
zestawKomputerowy->setHdd(wynik);
 
}
};
 
class ZestawABC996:public Builder {
public:
 
ZestawABC996():Builder(){
}
 
void buildMonitor(){
zestawKomputerowy->setMonitor("LG");
}
 
void buildProcesor(){
zestawKomputerowy->setProcesor("INTEL");
}
 
void buildGrafika(){
//zestaw nie obejmuje karty graficznej
}
 
void buildRam(){
zestawKomputerowy->setRam("DDR");
}
 
void buildHdd(){
zestawKomputerowy->setHdd("Samsung");
}
};
 
/* kierownik */
class Director {
private:
Builder* builder;
 
public:
void setBuilder(Builder* b){
builder = b;
}
 
ZestawKomputerowy getZestaw(){
return builder->getZestaw();
}
 
void skladaj(){
builder->newZestaw();
builder->buildMonitor();
builder->buildProcesor();
builder->buildHdd();
builder->buildRam();
builder->buildGrafika();
}
};
 
int main(){
Director* szef = new  Director();
 
 
Builder* builder = new  ZestawXT001();
Builder* builder2 = new  ZestawABC996();
 
cout<<"\nZESTAW 1\n";
szef->setBuilder(builder);
szef->skladaj();
ZestawKomputerowy zestaw1 = szef->getZestaw();
 
szef->setBuilder(builder2);
szef->skladaj();
ZestawKomputerowy zestaw2 = szef->getZestaw();
 
zestaw1.show();
cout << "\n\nZESTAW2\n" ;
zestaw2.show();
 
int x;
cin >>x;
}