using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ArrowController : MonoBehaviour
{
    private Transform PlayerTransform;
    private float     fAngleZ;
    private Vector3   vPivot;
    private bool      isOnMovableBlock = false;

    void Start()
    { 
    }
    
    void Update()
    {
        fAngleZ = transform.eulerAngles.z;
        
        ArrowBehavior();
    }

    private void ArrowBehavior()
    {
        if (Input.GetMouseButton(0))
        {
            Vector3 vMouseWorldPos = UnityEngine.Camera.main.ScreenToWorldPoint(Input.mousePosition);

            float fSize  = 0f;
            Vector3 vDir = Vector3.zero;

            if(false == isOnMovableBlock)
            {
                fSize = Vector3.Magnitude(transform.position - vMouseWorldPos);
                vDir  = Vector3.Normalize(transform.position - vMouseWorldPos);
            }
            else
            {
                fSize = Vector3.Magnitude(vPivot - vMouseWorldPos);
                vDir  = Vector3.Normalize(vPivot - vMouseWorldPos);
            }

            float fNewSize = Mathf.Clamp((fSize - 10f) * 3f, 0.1f, 1.0f);

            Vector3 vLook  = new Vector3(0f, 0f, 1f);
            Vector3 vRight = Vector3.Cross(vDir, vLook);
            
            transform.up    = vDir;
            transform.right = vRight;
            
            transform.localScale = new Vector3(1f, fNewSize, 1f);
        }

        transform.position = PlayerTransform.position;
    }

    public void SetPlayerTransform(Transform transform)
    {
        PlayerTransform = transform;
        vPivot = new Vector3(0f, transform.position.y, 0f);
    }

    public void SetState()
    {
        isOnMovableBlock = true;
    }

    public void AngleCorrection()
    {
        if (fAngleZ > 80f && fAngleZ < 280f)
        {
            transform.up = new Vector3(0f, 1f, 0f);
            transform.right = new Vector3(1f, 0f, 0f);
        }
    }
}
