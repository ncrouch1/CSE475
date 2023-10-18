package com.cse475team7.datacollection

import android.app.PendingIntent
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.content.pm.PackageManager
import android.hardware.usb.UsbDevice
import android.hardware.usb.UsbDeviceConnection
import android.hardware.usb.UsbManager
import android.os.Bundle
import android.widget.Button
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import java.io.FileOutputStream
import java.io.File
import android.util.Log
import android.widget.TextClock
import android.widget.TextView

import com.felhr.usbserial.*
import java.util.Date

class MainActivity : AppCompatActivity() {

    private val MYPERMISSIONSREQUESTWRITEEXTERNALSTORAGE = 123
    private val debug = true

    // throw away change
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        if (debug)
            Log.e("startup", "Created View")

        val manager: UsbManager = getSystemService (Context.USB_SERVICE) as UsbManager
        var fos: FileOutputStream? = null
        var serialDevice: UsbSerialDevice? = null

        val recordingIndicator = findViewById<TextView>(R.id.isRecording)
        recordingIndicator.text = "Not Recording"
        val startRecordingButton = findViewById<Button>(R.id.startRecordingButton)
        startRecordingButton.setOnClickListener {
            val devices = manager.deviceList
            if (debug)
                Log.e("init", "Init manager")
            if (!filesDir.exists()) {
                Log.w("making dirs", "making file path")
                filesDir.mkdirs()
            }
            for (device:UsbDevice in devices.values) {
                try {
                    if (UsbSerialDevice.isSupported(device)) {
                        var intent:PendingIntent = PendingIntent.getBroadcast(this, 0, Intent(BuildConfig.APPLICATION_ID + ".USB_PERMISSION"),
                            PendingIntent.FLAG_IMMUTABLE)
                        manager.requestPermission(device, intent)
                        var filter:IntentFilter = IntentFilter(BuildConfig.APPLICATION_ID + ".USB_PERMISSION")
                        while (!manager.hasPermission(device));
                        if (manager.hasPermission(device)) {
                            val connection: UsbDeviceConnection = manager.openDevice(device)
                            serialDevice = UsbSerialDevice.createUsbSerialDevice(device, connection)
                            val filename:String = "SensorData" + Date().toString() + ".txt"
                            fos = FileOutputStream(File(filesDir,filename))
                            if (debug)
                                Log.e("serial", "Created serial and fos")
                                recordingIndicator.text = "Recording"
                            break
                        }

                    }
                } catch (e: Exception) {
                    e.printStackTrace()
                }
            }


            if (debug)
                Log.e("reader", "created read callback")
            if (serialDevice != null && !serialDevice!!.isOpen) {
                serialDevice!!.open();
                serialDevice!!.setBaudRate(115200);
                serialDevice!!.setDataBits(UsbSerialInterface.DATA_BITS_8);
                serialDevice!!.setParity(UsbSerialInterface.PARITY_ODD);
                serialDevice!!.setFlowControl(UsbSerialInterface.FLOW_CONTROL_OFF);
            }
            serialDevice?.read {
                fos?.write(it)
            }
            if (debug)
                Log.w("start", "start button was pressed")
            Log.w("fos status", "fos is null? " +  (fos == null))
        }
        val stopRecordingButton = findViewById<Button>(R.id.stopRecordingButton)
        stopRecordingButton.setOnClickListener {
            if (serialDevice != null && serialDevice!!.isOpen) {
                serialDevice!!.close()
            }
            if (fos != null)
                fos!!.close()
            if (debug)
                Log.w("stop", "stop button was pressed")
            recordingIndicator.text = "Not Recording"
        }
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        if (requestCode == MYPERMISSIONSREQUESTWRITEEXTERNALSTORAGE) {
            if (grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                // Permission granted, you can perform the file operations here
                Toast.makeText(this,
                    "Write Permission Granted",
                    Toast.LENGTH_SHORT).show()
            } else {
                // Permission denied
                Toast.makeText(
                    this,
                    "Write permission is required to save data to storage",
                    Toast.LENGTH_SHORT
                ).show()
            }
        }
    }
}
