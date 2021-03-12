import 'package:bluejoystick/SelectBondedDevicePage.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import 'JoyPadPage.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      theme: ThemeData.dark(),
      home: MainScreen(),
    );
  }
}

class MainScreen extends StatefulWidget {
  @override
  _MainScreenState createState() => _MainScreenState();
}

class _MainScreenState extends State<MainScreen> {
  @override
  void initState() {
    super.initState();
   /* SystemChrome.setPreferredOrientations([
      DeviceOrientation.landscapeRight,
      DeviceOrientation.landscapeLeft,
    ]);

    */
  }

  @override
  Widget build(BuildContext context) {
    /*JoystickDirectionCallback onDirectionChanged(
        double degrees, double distance) {
      String data =
          "Degree : ${degrees.toStringAsFixed(2)}, distance : ${distance.toStringAsFixed(2)}";
      print(data);
      // writeData(data);
    }

    PadButtonPressedCallback padButtonPressedCallback(
        int buttonIndex, Gestures gesture) {
      String data = "buttonIndex : ${buttonIndex}";
      print(data);
      //writeData(data);
    }

     */

    return Scaffold(
      appBar: AppBar(
        title: const Text('Dragon 48'),
        actions: <Widget>[
          IconButton(
            icon: const Icon(Icons.bluetooth),
            onPressed:  () async {
              final BluetoothDevice selectedDevice =
              await Navigator.of(context).push(
                MaterialPageRoute(
                  builder: (context) {
                    return SelectBondedDevicePage(checkAvailability: false);
                  },
                ),
              );

              if (selectedDevice != null) {
                print('Connect -> selected ' + selectedDevice.address);
                _startPlay(context, selectedDevice);
              } else {
                print('Connect -> no device selected');
              }
            },
          ),
        ],
      ),
      body: Container(
        child: Row(
          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
          children: <Widget>[
            Text("Please get connection")
          ],
        ),
      ),
      bottomNavigationBar: BottomAppBar(
        child: Container(
          height: 30.0,
          color: Colors.white10,
          child: Row(
            children: <Widget>[
              new Text(" Status:" + ""),
            ],
          ),
        ),
      ),
    );
  }
  void _startPlay(BuildContext context, BluetoothDevice server) {
    Navigator.of(context).push(
      MaterialPageRoute(
        builder: (context) {
          return JoyPad(server: server);
        },
      ),
    );
  }
}
