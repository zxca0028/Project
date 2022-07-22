using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BackGroundController : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (transform.position.y + 20f < UnityEngine.Camera.main.gameObject.transform.position.y)
        {
            gameObject.SetActive(false);
        }
    }
}
