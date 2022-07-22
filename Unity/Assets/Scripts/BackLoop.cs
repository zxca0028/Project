using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BackLoop : MonoBehaviour
{
    // 배경이 자연스럽게 이어지기 위한 두 배경의 Y 간격 = 19.4

    private float hight;


    private void Awake()
    {
        //배경의 스크롤링을 위해 박스의 가로길이를 width로 사용
        BoxCollider2D backgroundCollider = GetComponent<BoxCollider2D>();
        hight = backgroundCollider.size.y;
        System.Console.WriteLine(hight);

    }

    // Update is called once per frame
    void Update()
    {

            Reposition();
    }

    public void Reposition()
    {
        Vector2 offset = new Vector2(0, hight * 2f);
        transform.position = (Vector2)transform.position + offset;
    }
}
