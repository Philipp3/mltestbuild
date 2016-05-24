# Verteilte Systeme HW9
**Name: Peringer Philipp**

**Mtrkl-Nr.: 1218721**
**Gruppe 3:**  Benjamin Gaugler, Max Lou Hartel-Kaduk, Philipp Peringer

Aufgabe         | Punkte
  ------------- | -------------
     a          | 4/4
     b          | 2/2
     c          | 2/2
     d          | 2/2
=>              | 10/10
## Aufgabe a)
- Server starten (Main.java)
- POST Anfrage mit curl senden:
```bash
curl -X POST -H "Content-type: application/json" -d
'{"method":"div","params":{"a":20,"b":5},"id":"test-2","jsonrpc":"2.0"}'
'http://localhost:8080/cs/calculate'
```
- GET Anfrage
 - allgemeiner Aufbau:
  - http://localhost:8080/cs/{add|sub|mult}/{val0}/{val1}
  - http://localhost:8080/cs/{lucas}/{val}
 - z.B.: http://localhost:8080/cs/add/5/3
 - http://localhost:8080/cs/lucas/10


- Alternative kann auch das Google Chrome-Plugin JaSON verwendet werden

- Ein, in Java geschriebener Client, befindet sich, in Form eines UnitTests, in `src/test/java/com/example/MyResourceTest.java`.

- POST Anfragen
  - erfolgen an:
    - http://localhost:8080/cs/calculate
  - übermittelt werden muss:
    - ein [JSONRPC2][jsonrpc2]Request
    - z.B.:
    ```json
    {
      "jsonrpc":"2.0",
      "method": "div",
      "params": {"a": 42, "b": 21 },
      "id":"id-1"
    }
    ```

## Aufgabe b)

Der Server verarbeitet mehrere Anfragen parallel. Kann einfach überprüft werden, indem 2 Anfragen mit curl an den Server gesendet werden, wobei "multitasktest" (in der ComputationService-Klasse) auf "true" gesetzt ist.
Zu beachten ist dass 2 Anfragen in verschiedenen Tabs, im selben Browser nicht parallel ausgeführt werden, da der Browser selbst die Anfragen sequentiell ausführt. (dieses Problem kann umgangen werden, indem die Anfragen von verschienden Browsern gesendet werden)

## Aufgabe c)

- WSDL - Web Service Description Language
- beschreibt die Implementierung eines web services -> URI
 - port
 - Methodennamen
 - Argumente
 - Datentypen
- WSDL ist im XML-Format -> für Maschinen und Menschen lesbar/verständlich
 - ermöglicht es Services dynamisch aufzurufen und zu verbinden

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<application xmlns="http://wadl.dev.java.net/2009/02">
    <grammars/>
    <resources base="http://localhost:8080/cs/">
        <resource path="/">
            <resource path="/{method}/{val0}/{val1}">
                <param xmlns:xs="http://www.w3.org/2001/XMLSchema" name="val0" style="template" type="xs:int"/>
                <param xmlns:xs="http://www.w3.org/2001/XMLSchema" name="val1" style="template" type="xs:int"/>
                <param xmlns:xs="http://www.w3.org/2001/XMLSchema" name="method" style="template" type="xs:string"/>
                <method id="binaryOperation" name="GET">
                    <response>
                        <representation mediaType="text/plain"/>
                    </response>
                </method>
            </resource>
        </resource>
    </resources>
</application>

```
- resource: beschreibt Ort für Service
 - base: Basisadresse
 - path: relative Adresse zu "base"
  - path enthält Eingabeparameter
- param: beschreibt Eingabeparameter
 - xmlns:xs: kennzeichnet dass die Elemente und Datentypen aus dem http://www.w3.org/2001/XMLSchema Namensraum kommmen, außerdem sollen Elemente und Datentypen aus diesem Namensraum mit "xs" beginnen
 - name: Bezeichnung
 - style: spezifiziert den Typ des Parameters, mögliche Werte: query, header und template
  - query: wird verwendet falls Paramter in URI mit "&" verknüpft werden
  - template: Parameter läuft über resource path
  - header: gibt einen HTTP-Header für die Verwendung in der Anfrage an
 - type: Type
- method: legt fest welche Methode aufgerufen wird und welche HTTP Method verwendet wird
 - response:
  - representation: beschreibt Aufbau der Antwort
   - mediaType: klassifiziert die Daten in Antwort


## Aufgabe d)

Für die Zeitmessung wurden die Lösungen für die früheren Aufgaben lediglich um zwei Aufrufe von System.nanoTime() und die Ausgabe der Differenz erweitert. Zur Messung der Member-basierten implementiertung wurde eine vor der Zeitmessung initialisierte Klasse mit einer simplen Addiermethode verwendet. Die Socket-basierte Lösung kommuniziert mit einem seperat laufenden Java-Programm am gleichen Computer. Server und Client der RMI-basierten Implemtierung laufen in der gleichen JVM. Um den Schwankungseffekt anderer Systemauslastungen zu reduzieren, wurden jeweils drei Messungen durchgeführt und Mittelwerte gebildet.  Die Resultate sind die folgenden:

Methode    |   Zeit
-----------|-----------
Webservice |  1ms - 3ms
Member-based| 1764 ns
Socket| 1150 Microsec.
RMI | 1645 ns.


An den Ergebnissen ist zu erkennen, dass es keinen nennenswerten Unterschied zwischen Member-basiertem Aufruf und RMI gibt, wenn beide RMI-Instanzen in der gleichen JVM arbeiten. Es ist zu erwarten, dass es größere Diskrepanzen gibt, wenn Daten über ein Netzwerkinterface übertragen werden müssen, wie es bei der Socket- und der Webservice-basierten Implementierung der Fall ist. Es ist unklar, was die vergleichsweise großen Schwankungen beim Webservice verursacht. Eine mögliche Erklärung ist der größere Overhead der Webservice-Bibliotheken, der zu einer erweiterten Schwankungsbreite bei der Ausführungszeit führen kann.


[jsonrpc2]: http://www.jsonrpc.org/specification
