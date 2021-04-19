using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using System.IO.Ports;
using System.Threading;

public class AnglesShow : MonoBehaviour
{
    public static SerialPort sp = new SerialPort("COM3", 9600);
    public Transform ActBase;
    public Transform Base;
    public Transform Arm;
    public int BaseRot=0;
    public int BaseAng=0;
    public int ArmRot=0;
    [SerializeField]
    private int offsetBaseRot = 0;
    [SerializeField]
    private int offsetBaseAng = 0;
    [SerializeField]
    private int offsetArmRot = 0;

    private void Start()
    {
        OpenConnection();
    }
    void Update()
    {
        BaseRot = (int)(Mathf.Abs(Base.localRotation.eulerAngles.x)) * offsetBaseRot;
        BaseAng = (int)(ActBase.localRotation.eulerAngles.z) * offsetBaseAng;
        ArmRot = (int)(Arm.localRotation.eulerAngles.x) * offsetArmRot;
        if (Input.GetKeyDown(KeyCode.A)){
            SendData();
        }
        if(Input.GetKeyDown(KeyCode.T)){
            StartCoroutine("SendAutoData");
        }

    }

    public void OpenConnection()
    {
        if(sp != null)
        {
            if (sp.IsOpen)
            {
                sp.Close();
                Debug.Log("Closing port");
            }
            else
            {
                sp.Open(); // open Serial Connection
                sp.ReadTimeout = 200;
                Debug.Log("Port Open");

            }
        }
    }

    public void SendData()
    {
        Debug.Log("Sending Data");
        string frame = ":0" +BaseAng.ToString() + ":0" + BaseRot.ToString() + ":0" + ArmRot.ToString()+":\n";
        sp.WriteLine(frame);
        
    }

    private void OnApplicationQuit()
    {
        sp.Close();
    }

    IEnumerator SendAutoData(){
        yield return new WaitForSeconds(0.1f);
        SendData();
        StartCoroutine("SendAutoData");
    }
}
