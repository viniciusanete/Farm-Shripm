#include <Medicoes.h>
#include <SPI.h>
#include <Ethernet.h>


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };   //Definição do MAC Address da Ethernet Shield
IPAddress ip(192, 168, 0, 100);                       // Definição do endereço IP que a Ethernet Shield terá na rede onde estiver conectada. Verifique a Máscara-de-subrede e todos os equipamentos conectados na rede para achar um IP compatível e livre.
EthernetServer server(80);                             // Porta em que a Ethernet Shield irá receber as requisições das páginas (o padrão WEB é a porta 80)
Medicoes medicao;

String HTTP_req; 
String URLValue;

void processaMedicao(byte tipo,  EthernetClient cl);
String getURLRequest(String *requisicao);
bool mainPageRequest(String *requisicao);
int numeroMaxEnumerado = 10;
void setup()
{
    Ethernet.begin(mac, ip); 
    server.begin();           
    Serial.begin(9600);       
}

void loop()
{
    EthernetClient client = server.available();

    if (client) { 
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) { 
                char c = client.read(); 
                HTTP_req += c;  
                if (c == '\n' && currentLineIsBlank ) { 

                    if (mainPageRequest(&HTTP_req)) {
                        URLValue = getURLRequest(&HTTP_req);
                                                 
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: application/json");
                        client.println("Connection: close");
                        client.println();
                        for (int i = 0; i <= numeroMaxEnumerado; i++){
                          processaMedicao(i, client);
                        }
                    } else {
                        client.println("HTTP/1.1 404 Not Found");
                    }
                    
          HTTP_req = "";    
                    break;
                }
                
                if (c == '\n') {
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    currentLineIsBlank = false;
                }
        
            }
        } 
        delay(1);     
        client.stop(); 
    } 
}


void processaMedicao(byte tipo, EthernetClient cl)
{

String cHTML;

    cHTML = "tipo";
    cHTML += "=";
    cHTML += tipo;
        if (URLValue.indexOf(cHTML) > -1) { 
           cHTML = medicao.RetornarObjeto(tipo, "");
        } 
    cl.println(cHTML);
}


String getURLRequest(String *requisicao) {
int inicio, fim;
String retorno;

  inicio = requisicao->indexOf("GET") + 3;
  fim = requisicao->indexOf("HTTP/") - 1;
  retorno = requisicao->substring(inicio, fim);
  retorno.trim();
  return retorno;
}

bool mainPageRequest(String *requisicao) {
String valor;
bool retorno = false;

  valor = getURLRequest(requisicao);
  valor.toLowerCase();

  if (valor == "/medida") {
     retorno = true;
  }
  if (valor.substring(0,8) == "/medida?") {
     retorno = true;
  }  

  return retorno;
}
