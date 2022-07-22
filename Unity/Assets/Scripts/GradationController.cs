using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GradationController : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        Vector3 vCamPos = UnityEngine.Camera.main.transform.position;

        vCamPos.z = 0f;

        transform.position = vCamPos;
    }
}
