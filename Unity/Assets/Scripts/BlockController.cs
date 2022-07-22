using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BlockController : MonoBehaviour
{
    private PlayerController Player;
    private bool isMoving = false;

    void Start()
    {
        Player = GameObject.FindObjectOfType<PlayerController>();
    }

    void Update()
    {
        if(true == isMoving)
        {
            MovableBlock();
        }

        if(transform.position.y + 15f < UnityEngine.Camera.main.gameObject.transform.position.y)
        {
            Destroy(gameObject);
        }
    }

    void MovableBlock()
    {
        transform.RotateAround(Vector3.up, Vector3.up, 100 * Time.deltaTime);
        transform.right = Vector3.right;
    }

    public void SetMoveState()
    {
        isMoving = true;
    }
    public bool GetState()
    {
        return isMoving;
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        Player.SetState(isMoving);
    }
}
