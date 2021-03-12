import 'dart:convert';
import 'package:control_pad/control_pad.dart';
import 'package:control_pad/models/gestures.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';

class JoyPad extends StatefulWidget {
  final BluetoothDevice server;
  const JoyPad({this.server});

  @override
  _JoyPadState createState() => _JoyPadState();
}

class _JoyPadState extends State<JoyPad> {
  BluetoothConnection connection;
  bool isConnecting = true;
  bool get isConnected => connection != null && connection.isConnected;
  bool isDisconnecting = false;
  @override
  void initState() {
    super.initState();
    SystemChrome.setPreferredOrientations([
      DeviceOrientation.landscapeRight,
      DeviceOrientation.landscapeLeft,
    ]);

    BluetoothConnection.toAddress(widget.server.address).then((_connection) {
      print('Connected to the device');
      connection = _connection;
      setState(() {
        isConnecting = false;
        isDisconnecting = false;
      });
      /*
      connection.input.listen(_onDataReceived).onDone(() {
        // Example: Detect which side closed the connection
        // There should be `isDisconnecting` flag to show are we are (locally)
        // in middle of disconnecting process, should be set before calling
        // `dispose`, `finish` or `close`, which all causes to disconnect.
        // If we except the disconnection, `onDone` should be fired as result.
        // If we didn't except this (no flag set), it means closing by remote.
        if (isDisconnecting) {
          print('Disconnecting locally!');
        } else {
          print('Disconnected remotely!');
        }
        if (this.mounted) {
          setState(() {});
        }
      });

       */
    }).catchError((error) {
      print('Cannot connect, exception occured');
      print(error);
    });
  }

  @override
  Widget build(BuildContext context) {
    JoystickDirectionCallback onDirectionChanged(double degrees,
        double distance) {
      String data =
          "Degree : ${degrees.toStringAsFixed(2)}, distance : ${distance
          .toStringAsFixed(2)}";
      print(data);
      // writeData(data);
      _sendMessage(data);
    }

    PadButtonPressedCallback padButtonPressedCallback(int buttonIndex,
        Gestures gesture) {
      String data = "buttonIndex : ${buttonIndex}";
      print(data);
      //writeData(data);
      _sendMessage(data);
    }

    return Scaffold(
      appBar: AppBar(
        title: const Text('Dragon 48'),
        actions: <Widget>[
          IconButton(
            icon: const Icon(Icons.bluetooth),
            onPressed:null,
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
  void _sendMessage(String text) async {
    text = text.trim();
    //textEditingController.clear();

    if (text.length > 0) {
      try {
        connection.output.add(utf8.encode(text + "\r\n"));
        await connection.output.allSent;
        /*
        setState(() {
          messages.add(_Message(clientID, text));
        });

        Future.delayed(Duration(milliseconds: 333)).then((_) {
          listScrollController.animateTo(
              listScrollController.position.maxScrollExtent,
              duration: Duration(milliseconds: 333),
              curve: Curves.easeOut);
        });

         */
      } catch (e) {
        // Ignore error, but notify state
        setState(() {});
      }
    }
  }
}