using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameOverMenu : MonoBehaviour
{

    // Start is called before the first frame update
    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {
        Vector3 vCamWorldPos   = UnityEngine.Camera.main.transform.position;
        Vector3 vMouseWorldPos = UnityEngine.Camera.main.ScreenToWorldPoint(Input.mousePosition);

        vMouseWorldPos.z = 0f;

        Vector3 vDir = vMouseWorldPos - vCamWorldPos;
        
        RaycastHit hit;

        if(Physics.Raycast(vCamWorldPos, vDir, out hit))
        {
            Debug.Log(hit.point);
        }

        Debug.Log("CamWorldPos : " + vCamWorldPos);
        Debug.Log("MouseWorldPos : " + vMouseWorldPos);
    }
}
