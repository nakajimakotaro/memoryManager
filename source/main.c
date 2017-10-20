#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memoryManager.h"

enum Operation
{
    Operation_LIST,
    Operation_ADD,
    Operation_WRITE,
    Operation_CLEAR,
    Operation_EXIT,
    Operation_ERROR,
};



struct _Character
{
    char name[100];
    int hp;
    int mp;
    struct _Character *next;
};

typedef struct _Character Character;

void characterListAdd(Character *root, Character *addChara)
{
    Character *last = root;
    if (root == NULL)
    {
        root = addChara;
        return;
    }
    while (last->next)
    {
        last = last->next;
    }
    last->next = addChara;
}

Character *newCharacter(char *name, int hp, int mp)
{
    Character *chara = malloc(sizeof(Character));
    strncpy(chara->name, name, 100);
    chara->hp = hp;
    chara->mp = mp;
    chara->next = NULL;
    return chara;
}
Character *characterListRemove(Character *root, int index)
{
    Character *last;
    Character *removePrevChara = root;
    for (int i = 0; i < index - 1; i++)
    {
        removePrevChara = removePrevChara->next;
    }
    Character *delete = removePrevChara->next;
    removePrevChara->next = removePrevChara->next->next;
    return delete;
}
typedef struct _Character Character;

void view(Character *root)
{
    printf("-----------------------------------------------\n");

    int count = 0;
    for (Character *current = root; current != NULL; current = current->next)
    {
        printf("|番号| %d|| 名前| %-10s||hp| %3d||mp| %3d||\n", count, current->name, current->hp, current->mp);
        count++;
    }
    printf("-----------------------------------------------\n");
}
void getInteractiveCharacter(Character *chara)
{
    char buf[100];
    int index = 0;
    char name[11] = "";
    int hp = 0;
    int mp = 0;
    printf("名前(1バイト文字のみで10字以内): ");
    fgets(buf, 100, stdin);
    sscanf_s(buf, "%s", name, 11);
    name[10] = '\0';
    printf("\n");

    printf("HP: ");
    fgets(buf, 100, stdin);
    sscanf_s(buf, "%d", &hp);
    printf("\n");

    printf("MP: ");
    fgets(buf, 100, stdin);
    sscanf_s(buf, "%d", &mp);
    printf("\n");

    strncpy(chara->name, name, 11);
    chara->hp = hp;
    chara->mp = mp;
    chara->next = NULL;
}
Character *add(Character *root)
{
    Character *addChara = newCharacter("", 0, 0);
    getInteractiveCharacter(addChara);
    if (root != NULL)
    {
        characterListAdd(root, addChara);
    }
    else
    {
        root = addChara;
    }
    return root;
}
Character *selectChara(Character *root, int index)
{
    Character *selectChara = root;
    for (int i = 0; i < index; i++)
    {
        if (selectChara->next == NULL)
        {
            return NULL;
        }
        selectChara = selectChara->next;
    }
    return selectChara;
}
void write(Character *root)
{
    printf("書き換える番号: ");
    char buf[100];
    int index;
    fgets(buf, 100, stdin);
    sscanf_s(buf, "%d", &index);
    printf("\n");

    Character *chara = selectChara(root, index);
    if (chara == NULL)
    {
        return;
    }
    getInteractiveCharacter(chara);
}

void clear(Character *root)
{
    Character *current = root;
    if (current->next == NULL)
    {
        free(current);
    }
    else
    {
        clear(current->next);
        free(current);
    }
}

int getOperationCode(char *input)
{
    char operationCode = input[0];
    if (operationCode == 'l')
    {
        return Operation_LIST;
    }
    if (operationCode == 'a')
    {
        return Operation_ADD;
    }
    if (operationCode == 'w')
    {
        return Operation_WRITE;
    }
    if (operationCode == 'c')
    {
        return Operation_CLEAR;
    }
    if (operationCode == 'e')
    {
        return Operation_EXIT;
    }
    else
    {
        return Operation_ERROR;
    }
}

void main()
{

    Character *chara;
    Character *root;
    root = newCharacter("musigidane", 10, 10);
    chara = newCharacter("musigisou", 20, 20);
    characterListAdd(root, chara);

    chara = newCharacter("musigibana", 30, 30);
    characterListAdd(root, chara);

    int isLoop = 1;
    while (isLoop)
    {
        char input[100];
        printf("l|list: 表\示, a|add: 追加, w|write: 上書き編集, c|clear: 削除, e|exit: 終了\n");
        fgets(input, 100, stdin);
        switch (getOperationCode(input))
        {
        case Operation_LIST:
            view(root);
            break;
        case Operation_ADD:
            root = add(root);
            break;
        case Operation_WRITE:
            write(root);
            break;
        case Operation_CLEAR:
            clear(root);
            root = NULL;
            break;
        case Operation_EXIT:
            isLoop = 0;
            break;
        case Operation_ERROR:
            printf("入力コードが解釈できません\n");
            break;
        }
    }
}
