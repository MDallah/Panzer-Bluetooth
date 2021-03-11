import 'package:flutter/material.dart';
import 'package:control_pad/control_pad.dart';
import 'package:control_pad/models/gestures.dart';
import 'package:flutter/services.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      theme: ThemeData.dark(),
      home: JoyPad(),
    );
  }
}

class JoyPad extends StatefulWidget {
  @override
  _JoyPadState createState() => _JoyPadState();
}

class _JoyPadState extends State<JoyPad> {
  @override
  void initState() {
    super.initState();
    SystemChrome.setPreferredOrientations([
      DeviceOrientation.landscapeRight,
      DeviceOrientation.landscapeLeft,
    ]);
  }

  @override
  Widget build(BuildContext context) {
    JoystickDirectionCallback onDirectionChanged(
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

    return Scaffold(
      appBar: AppBar(
        title: const Text('Dragon 48'),
        actions: <Widget>[
          IconButton(
            icon: const Icon(Icons.bluetooth),
            onPressed: null,
          ),
        ],
      ),
      body: Container(
        child: Row(
          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
          children: <Widget>[
            PadButtonsView(
              padButtonPressedCallback: padButtonPressedCallback,
            ),
            JoystickView(
              onDirectionChanged: onDirectionChanged,
            ),
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
}
