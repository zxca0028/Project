using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GaugeController : MonoBehaviour
{
    private enum STATE
    {
        STATE_INCREASE,
        STATE_DECREASE,
        STATE_END
    }

    private STATE eState;

    private float fCurrentX = 0f;

    // 게이지 속도 : 1f ~ 3f;
    // public으로 풀면 이상해짐 이유는 몰?루
    private float fGaugeSpeed = 1f;

    bool bStop = false;

    public Transform bar;

    // Start is called before the first frame update
    void Start()
    {
        bar = transform.Find("Bar");
        bar.localScale = new Vector3(0.3f, 1f);
        eState = STATE.STATE_INCREASE;
    }

    // Update is called once per frame
    void Update()
    {
        if(false == bStop)
        {
            fCurrentX = bar.localScale.x;

            GaugeBehavior();
        }
    }

    public void Stop()
    {
        bStop = true;
    }

    private void GaugeBehavior()
    {
        switch (eState)
        {
            case STATE.STATE_INCREASE:

                fCurrentX += Time.deltaTime * fGaugeSpeed;
                bar.localScale = new Vector3(fCurrentX, 1f);

                if(Mathf.Clamp(fCurrentX, 0.1f, 1f) >= 1f)
                {
                    eState = STATE.STATE_DECREASE;
                }

                break;

            case STATE.STATE_DECREASE:

                fCurrentX -= Time.deltaTime * fGaugeSpeed;
                bar.localScale = new Vector3(fCurrentX, 1f);

                if (Mathf.Clamp(fCurrentX, 0.1f, 1f) <= 0.1f)
                {
                    eState = STATE.STATE_INCREASE;
                }

                break;
        }
    }
}
