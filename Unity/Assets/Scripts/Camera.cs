using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Camera : MonoBehaviour
{
    public PlayerController PlayerObject;

    Vector3 CameraVec;

    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {
        if(PlayerObject.transform.position.y >= 6f)
        {
            if(PlayerObject.transform.position.y > transform.position.y)
            {
                transform.Translate(0f, Time.deltaTime * 3f, 0f);
            }
        }

        //CameraVec.z = PlayerObject.transform.position.z - 10f;
        //CameraVec.y = PlayerObject.transform.position.y + 8f;
        //transform.position = CameraVec;   
    }
}
