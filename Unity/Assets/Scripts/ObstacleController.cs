using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObstacleController : MonoBehaviour
{
    private enum STATE
    {
        STATE_LEFT  =  1,
        STATE_RIGHT = -1,
        STATE_END
    }

    private STATE eState = STATE.STATE_LEFT;
    
    private float fRange = 0f;
    private Vector3 vScale;


    // Start is called before the first frame update
    void Start()
    {
        fRange = Mathf.Abs(transform.position.x);
        vScale = transform.localScale;
    }

    // Update is called once per frame
    void Update()
    {
        Vector3 vAxisY = new Vector3(0f, 1f, 0f);

        transform.RotateAround(vAxisY, Vector3.up, 100 * Time.deltaTime);

        transform.right = new Vector3(1f, 0f, 0f);

        ObstacleBehavior();
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        GameManager.instance.OnplayerDead();

        Destroy(gameObject);
    }

    private void ObstacleBehavior()
    {
        switch (eState)
        {
            case STATE.STATE_LEFT:

                if (transform.localScale.x < 0)
                {
                    vScale.x *= -1f;

                    transform.localScale = vScale;
                }

                if (transform.position.x <= -fRange + 0.05f)
                {
                    eState = STATE.STATE_RIGHT;
                }

                break;

            case STATE.STATE_RIGHT:

                if (transform.localScale.x > 0)
                {
                    vScale.x *= -1f;

                    transform.localScale = vScale;
                }

                if (transform.position.x >= fRange - 0.05f)
                {
                    eState = STATE.STATE_LEFT;
                }

                break;
        }
    }
}
