using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DeadLineController : MonoBehaviour
{
    public Camera Cam;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        Vector3 vCurrentPos = transform.position;

        vCurrentPos.y = Cam.transform.position.y - 5.5f;
        
        transform.position = vCurrentPos;
    }
}
