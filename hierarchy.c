#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Queue.h"
#include "hierarchy.h"

int MAX(int a, int b)
{

  if (a > b)
    return a;
  return b;
}

Tree search(Tree tree, char *manager)
{

  if (tree == NULL)
    return NULL;

  if (tree != NULL)
  {

    if (strcmp(tree->name, manager) == 0)
    {

      return tree;
    }
    for (int i = 0; i < (tree)->direct_employees_no; i++)
    {

      Tree aux = search(tree->team[i], manager);

      if (aux != NULL)
        return aux;
    }
  }
  return NULL;
}

/* Adauga un angajat nou in ierarhie.
 *
 * tree: ierarhia existenta
 * employee_name: numele noului angajat
 * manager_name: numele sefului noului angajat
 *
 * return: intoarce ierarhia modificata. Daca tree si manager_name sunt NULL,
           atunci employee_name e primul om din ierarhie.
 */

Tree hire(Tree tree, char *employee_name, char *manager_name)
{

  if (tree == NULL && manager_name == NULL)
  { // adaugam radacina

    Tree Employee = (Tree)malloc(sizeof(TreeNode)); // alocam spatiu pentru angajat

    if (Employee == NULL)
    {
      printf("Malloc Failed !");
      exit(1);
    }

    Employee->name = employee_name;
    Employee->manager = NULL;
    Employee->direct_employees_no = 0;
    Employee->team = NULL;

    return Employee;
  }
  else
  {

    Tree Employee = (Tree)malloc(sizeof(TreeNode));

    if (Employee == NULL)
    {
      printf("Malloc Failed !");
      exit(1);
    }

    Employee->name = employee_name;
    Employee->manager = NULL;
    Employee->direct_employees_no = 0;
    Employee->team = NULL;

    Tree managerNode = NULL;

    managerNode = search(tree, manager_name);

    if (managerNode == NULL)
    {

      return tree;
    }

    (managerNode->direct_employees_no)++;
    managerNode->team = realloc(managerNode->team, (managerNode->direct_employees_no) * sizeof(Tree));

    if (managerNode->team == NULL)
    {
      printf("Malloc Failed !");
      exit(1);
    }

    if (managerNode->team == NULL)
    {

      return tree;
    }

    int k = managerNode->direct_employees_no;

    if (k != 1)
    { // daca managerul mai are alti anagajti in echipa

      if (strcmp(managerNode->team[k - 2]->name, employee_name) < 0)
      { // verificam daca putem inseara pe ultima pozitie

        managerNode->team[k - 1] = Employee;
        Employee->manager = managerNode;
      }
      else
      {

        for (int i = (k - 2); i >= 0; i--)
        { // altfel inseram la mijloc

          if (strcmp(employee_name, managerNode->team[i]->name) < 0)
          {

            managerNode->team[i + 1] = managerNode->team[i];

            managerNode->team[i] = Employee;
          }
        }

        Employee->manager = managerNode;
      }
    }
    else
    { // altfel inseram la inceput

      managerNode->team[0] = Employee;
      Employee->manager = managerNode;
    }
  }

  return tree;
}

/* Sterge un angajat din ierarhie.
 *
 * tree: ierarhia existenta
 * employee_name: numele angajatului concediat
 *
 * return: intoarce ierarhia modificata.
 */
Tree fire(Tree tree, char *employee_name)
{

  Tree ToFire = search(tree, employee_name);

  if (ToFire != NULL && ToFire->manager != NULL)
  { // verificam sa existe anagajatul si sa nu fie radacina

    if (ToFire->direct_employees_no == 0)
    { // daca anagajatul de concediat nu are "copii"

      int i = 0;

      for (i = 0; i < ToFire->manager->direct_employees_no; i++)
      { // il stergem din vectorul echipa al managerului sau

        if (strcmp(ToFire->manager->team[i]->name, employee_name) == 0)
        {

          ToFire->manager->direct_employees_no--;

          for (int j = i; j < ToFire->manager->direct_employees_no; j++)
          {

            ToFire->manager->team[j] = ToFire->manager->team[j + 1];
          }
        }
      }

      destroy_tree(ToFire);
    }
    else
    { // altfel ne asiguram ca anagajatii sai subordonati sa ajunga in echipa managerului curent

      Tree *arrEmployees = ToFire->team;
      int sizeArrEmployees = ToFire->direct_employees_no;

      for (int k = 0; k < sizeArrEmployees; k++)
      {
        ToFire->team[k]->manager = ToFire->manager;
      }

      int i = 0;

      for (i = 0; i < ToFire->manager->direct_employees_no; i++)
      { // il stergem din vectorul echipa al managerului sau

        if (strcmp(ToFire->manager->team[i]->name, employee_name) == 0)
        {

          ToFire->manager->direct_employees_no--;

          for (int j = i; j < ToFire->manager->direct_employees_no; j++)
          {

            ToFire->manager->team[j] = ToFire->manager->team[j + 1];
          }
        }
      }

      int initialSize = ToFire->manager->direct_employees_no;

      ToFire->manager->team = realloc(ToFire->manager->team, (initialSize + sizeArrEmployees) * sizeof(Tree));

      if (ToFire->manager->team == NULL)
      {
        printf("Realloc Failed !");
        exit(1);
      }

      ToFire->manager->direct_employees_no = initialSize + sizeArrEmployees;

      int q = 0;
      for (int p = 0; p < ToFire->manager->direct_employees_no; p++)
      {
        if (p == initialSize)
        {

          ToFire->manager->team[p] = arrEmployees[q];
          q++;
          initialSize++;
        }
      }

      Tree temp = NULL;

      for (int p = 0; p <= (ToFire->manager->direct_employees_no - 1); p++)
      { // dupa ce am adaugat si restul de angajati in noua echipa, sortam alfabetic
        for (int q = p + 1; q <= (ToFire->manager->direct_employees_no - 1);
             q++)
        {
          if (strcmp(ToFire->manager->team[p]->name,
                     ToFire->manager->team[q]->name) > 0)
          {

            temp = ToFire->manager->team[p];

            ToFire->manager->team[p] = ToFire->manager->team[q];

            ToFire->manager->team[q] = temp;
          }
        }
      }

      free(arrEmployees);
      free(ToFire);
    }
  }
  else
  {
    return tree;
  }

  return tree;
}

/* Promoveaza un angajat in ierarhie. Verifica faptul ca angajatul e cel putin
 * pe nivelul 2 pentru a putea efectua operatia.
 *
 * tree: ierarhia existenta
 * employee_name: numele noului angajat
 *
 * return: intoarce ierarhia modificata.
 */
Tree promote(Tree tree, char *employee_name)
{

  Tree ToPromo = search(tree, employee_name);
  Tree Manager = NULL;

  if (ToPromo->manager != NULL)
    Manager = ToPromo->manager->manager;

  if (ToPromo->manager != tree && ToPromo->manager != NULL)
  {

    fire(tree, ToPromo->name);

    hire(tree, employee_name, Manager->name);
  }
  return tree;
}

/* Muta un angajat in ierarhie.
 *
 * tree: ierarhia existenta
 * employee_name: numele angajatului
 * new_manager_name: numele noului sef al angajatului
 *
 * return: intoarce ierarhia modificata.
 */
Tree move_employee(Tree tree, char *employee_name, char *new_manager_name)
{

  Tree ToMove = search(tree, employee_name);

  if (ToMove->manager != NULL &&
      (strcmp(ToMove->manager->name, new_manager_name) != 0))
  {

    tree = fire(tree, employee_name);
    tree = hire(tree, employee_name, new_manager_name);
  }
  return tree;
}

/* Muta o echipa in ierarhie.
 *
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei mutate
 * new_manager_name: numele noului sef al angajatului
 *
 * return: intoarce ierarhia modificata.
 */
Tree move_team(Tree tree, char *employee_name, char *new_manager_name)
{

  Tree TToMove = search(tree, employee_name);

  if (TToMove != NULL)
  {

    if (TToMove->manager != NULL)
    {

      int i = 0;

      for (i = 0; i < TToMove->manager->direct_employees_no; i++)
      { // stergem din echipa initiala

        if (strcmp(TToMove->manager->team[i]->name, employee_name) == 0)
        {

          TToMove->manager->direct_employees_no--;

          for (int j = i; j < TToMove->manager->direct_employees_no; j++)
          {
            TToMove->manager->team[j] = TToMove->manager->team[j + 1];
          }
        }
      }

      if (TToMove->direct_employees_no == 0)
      { // daca nu are copii

        hire(tree, employee_name, new_manager_name);
        destroy_tree(TToMove);

        return tree;
      }
      else
      { // daca are copii

        Tree NewManager = search(tree, new_manager_name);

        NewManager->direct_employees_no++;
        NewManager->team = realloc(NewManager->team, (NewManager->direct_employees_no) * sizeof(Tree));

        if (NewManager->team == NULL)
        {
          printf("Realloc Failed !");
          exit(1);
        }

        int k = NewManager->direct_employees_no;

        if (k != 1)
        {

          if (strcmp(NewManager->team[k - 2]->name, employee_name) < 0)
          {

            NewManager->team[k - 1] = TToMove;
            TToMove->manager = NewManager;
          }
          else
          {

            for (int i = (k - 2); i >= 0; i--)
            {

              if (strcmp(TToMove->name, NewManager->team[i]->name) < 0)
              {

                NewManager->team[i + 1] = NewManager->team[i];

                NewManager->team[i] = TToMove;
              }
            }

            TToMove->manager = NewManager;
          }
        }
        else
        {
          NewManager->team[0] = TToMove;
          TToMove->manager = NewManager;
        }
      }
    }
  }
  return tree;
}

/* Concediaza o echipa din ierarhie.
 *
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei concediate
 *
 * return: intoarce ierarhia modificata.
 */
Tree fire_team(Tree tree, char *employee_name)
{

  Tree ToFire = search(tree, employee_name);

  if (ToFire->manager != NULL)
  {

    int i = 0;

    for (i = 0; i < ToFire->manager->direct_employees_no; i++)
    {

      if (strcmp(ToFire->manager->team[i]->name, employee_name) == 0)
      {

        ToFire->manager->direct_employees_no--;

        for (int j = i; j < ToFire->manager->direct_employees_no; j++)
        {
          ToFire->manager->team[j] = ToFire->manager->team[j + 1];
        }
      }
    }
    destroy_tree(ToFire);
  }

  return tree;
}

/* Afiseaza toti angajatii sub conducerea unui angajat.
 *
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei
 */
Tree *getEmployees(Tree *arr, Tree tree)
{

  if (tree != NULL)
  {

    arr[arr[0]->direct_employees_no] = tree;
    arr[0]->direct_employees_no++;

    for (int i = 0; i < tree->direct_employees_no; i++)
      getEmployees(arr, tree->team[i]);
  }
  return arr;
}

int numberOfEmpl(Tree tree)
{
  int size = 1;

  if (tree == NULL)
    return 0;

  for (int i = 0; i < tree->direct_employees_no; i++)
  {

    size += numberOfEmpl(tree->team[i]);
  }
  return size;
}

void get_employees_by_manager(FILE *f, Tree tree, char *employee_name)
{

  Tree ToGet = search(tree, employee_name);

  if (ToGet == NULL)
  {
    fprintf(f, "\n");
    return;
  }

  if (ToGet != NULL)
  {

    int sizeArray = numberOfEmpl(ToGet);

    Tree *emplArr = malloc((sizeArray + 1) * sizeof(Tree));

    emplArr[0] = malloc(sizeof(TreeNode)); // "decoy" folosit strict pentru incrementare in functia getEmployees

    if (emplArr[0] == NULL)
    {
      printf("Malloc Decoy Failed !");
      exit(1);
    }

    emplArr[0]->direct_employees_no = 1;

    emplArr = getEmployees(emplArr, ToGet); // plasam toti anagajatii subordonati direct si indirect intr-un vector

    Tree temp;
    Tree *tempVec = NULL;

    for (int p = 1; p <= (sizeArray - 1); p++)
    { // sortam alfabetic vectorul
      for (int q = p + 1; q <= (sizeArray); q++)
      {
        if (strcmp(emplArr[p]->name, emplArr[q]->name) > 0)
        {

          temp = emplArr[p];

          emplArr[p] = emplArr[q];

          emplArr[q] = temp;
        }
      }
    }

    for (int i = 1; i <= (sizeArray); i++)
    { // afisam
      if (i == sizeArray)
        fprintf(f, "%s \n", emplArr[i]->name);
      else
        fprintf(f, "%s ", emplArr[i]->name);
    }

    free(emplArr[0]);
    free(emplArr);

    return;
  }
  return;
}

/* Afiseaza toti angajatii de pe un nivel din ierarhie.
 *
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 * level: nivelul din ierarhie
 */

int maxDepth(Tree tree)
{ // returneza inaltimea maxima a arborelui.
  if (tree == NULL)
    return 0;

  int depth = 0;

  for (int i = 0; i < tree->direct_employees_no; i++)
  {
    depth = MAX(depth, maxDepth(tree->team[i]));
  }

  return depth + 1;
}

void sortArr(Tree *arr, int size)
{ // sorteaza alfabetic un vector
  Tree temp = NULL;
  for (int p = 0; p <= (size - 1); p++)
  {
    for (int q = p + 1; q <= (size); q++)
    {
      if (strcmp(arr[p]->name, arr[q]->name) > 0)
      {

        temp = arr[p];

        arr[p] = arr[q];

        arr[q] = temp;
      }
    }
  }
  return;
}

void get_employees_by_level(FILE *f, Tree tree, int level)
{

  int maxdepth = maxDepth(tree);

  if (level < 0 || level > (maxdepth - 1))
  { // verificam daca exista level transmis ca parametru al functiei
    fprintf(f, "\n");
    return;
  }
  else
  {

    if (tree == NULL)
      return;

    int lvl = 0;

    int sizeArray = 0;
    int indexVec = 0;

    Tree *emplArr = malloc((sizeArray + 1) * sizeof(Tree));

    if (emplArr == NULL)
    {
      printf("Malloc Employees Team Failed !");
      exit(1);
    }

    Queue *empQ = createQueue();

    if (empQ == NULL)
    {
      printf("Queue Creation Failed !");
      exit(1);
    }

    enqueue(empQ, tree);

    int done = 0;

    if (lvl == level)
    {
      emplArr[indexVec] = tree;
      indexVec++;
      sizeArray++;
      done = 1;
    }

    int k = 0;

    while (isQueueEmpty(empQ) != 1)
    {

      int size = sizeQ(empQ);

      while (size > 0)
      {

        Tree elem = front(empQ);

        dequeue(empQ);

        emplArr = realloc(emplArr, (sizeArray + 1) * sizeof(Tree)); // pe masura ce preluam un element din coada , marim vectorul cu o unitate si stocam angajatul

        if (emplArr == NULL)
        {
          printf("Realloc Employees Team Failed !");
          exit(1);
        }

        if (lvl == level && done == 0)
        {
          emplArr[indexVec] = elem;
          indexVec++;
          sizeArray++;
        }

        for (int i = 0; i < elem->direct_employees_no; i++)
          enqueue(empQ, elem->team[i]);

        size--;
      }

      lvl++;
    }

    sortArr(emplArr, sizeArray - 1);

    for (int i = 0; i < sizeArray; i++)
    {
      fprintf(f, "%s ", emplArr[i]->name);
    }
    fprintf(f, "\n");

    free(emplArr);
    destroyQueue(empQ);
    free(empQ);
  }

  return;
}

/* Afiseaza angajatul cu cei mai multi oameni din echipa.
 *
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 */

void get_best_manager(FILE *f, Tree tree)
{

  int sizeArray = numberOfEmpl(tree);
  Tree *emplVec = malloc((sizeArray + 1) * sizeof(Tree));

  emplVec[0] = malloc(sizeof(TreeNode)); // "decoy" pentru incrementare

  if (emplVec[0] == NULL)
  {
    printf("Malloc Decoy Failed !");
    exit(1);
  }

  emplVec[0]->direct_employees_no = 1;

  emplVec = getEmployees(emplVec, tree); // stocam anagajatii in vector

  int max = emplVec[1]->direct_employees_no;
  int pos[20] = {0}; // vector de frecventa

  int k = 2;
  for (int i = 2; i < sizeArray; i++)
  { // pe masura ce gasim un maxim actualizam si vectorul de frecventa
    if (emplVec[i]->direct_employees_no >= max)
    {

      max = emplVec[i]->direct_employees_no;
      pos[k] = i;
      k++;
    }
  }

  if (max == emplVec[1]->direct_employees_no)
    fprintf(f, "%s ", emplVec[1]->name);
  for (int i = 0; i < 20; i++)
  { // daca sunt mai multi manager cu acelasi maxim
    if (pos[i] != 0)
      fprintf(f, "%s ", emplVec[pos[i]]->name);
  }
  fprintf(f, "\n");

  free(emplVec[0]);
  free(emplVec);
  return;
}

/* Reorganizarea ierarhiei cu un alt angajat in varful ierarhiei.
 *
 * tree: ierarhia existenta
 * employee_name: numele angajatului care trece in varful ierarhiei
 */
void addToTeam(Tree managerNode, Tree *arr, int size, Tree Employee)
{ // adauga un employee in vectorul "team" all managerului transmis ca param

  int pos = 0;
  int k = managerNode->direct_employees_no;

  if (k != 1)
  {

    if (strcmp(managerNode->team[k - 2]->name, Employee->name) < 0)
    {

      managerNode->team[k - 1] = Employee;
      Employee->manager = managerNode;
    }
    else
    {

      for (int i = (k - 2); i >= 0; i--)
      {

        if (strcmp(Employee->name, managerNode->team[i]->name) < 0)
        {

          managerNode->team[i + 1] = managerNode->team[i];

          managerNode->team[i] = Employee;
        }
      }
      Employee->manager = managerNode;
    }
  }
  else
  {

    managerNode->team[0] = Employee;
    Employee->manager = managerNode;
  }
}

Tree fire_team2(Tree tree, char *employee_name, Tree original_manager)
{ // exact ca functia fire() doar ca aici se specifica managerul din echipa caruia vrem sa eliminam

  Tree ToFire = search(tree, employee_name);
  Tree Manager = search(tree, original_manager->name);

  if (ToFire->manager != NULL)
  {

    int i = 0;

    for (i = 0; i < Manager->direct_employees_no; i++)
    {

      if (strcmp(Manager->team[i]->name, employee_name) == 0)
      {

        Manager->direct_employees_no--;

        for (int j = i; j < Manager->direct_employees_no; j++)
        {
          Manager->team[j] = Manager->team[j + 1];
        }
      }
    }
  }
  return tree;
}

Tree reorganize(Tree tree, char *employee_name)
{

  Tree ToReorg = search(tree, employee_name);

  if (ToReorg != NULL && strcmp(tree->name, employee_name) != 0)
  { // daca exista si daca nu este radacina

    Tree current = ToReorg;

    int step = 0;
    Tree last = current;
    Tree originalManager = current->manager;
    Tree newRoot = NULL;

    while (originalManager != NULL)
    {

      fire_team2(tree, current->name, originalManager); // separam de arbore nodul vrut

      current->direct_employees_no++;
      current->team = realloc(current->team, (current->direct_employees_no) * sizeof(Tree)); // ii alocam un spatiu in echipa unde vom pune managerul sau

      last = originalManager;
      if (originalManager != NULL)
        originalManager = originalManager->manager;

      addToTeam(current, current->team, current->direct_employees_no, last); // adaugam in echipa managerul sau

      if (step == 0)
      { // daca ne aflam la primul pas , actualizam radacina arborelui care va fi respectivul employee transmis ca param

        current->manager = NULL;
        newRoot = current;
        step++;
      }
      current = last; // actualizam nodul la managerul sau.
    }
    return newRoot;
  }

  return tree;
}

/* Parcurge ierarhia conform parcurgerii preordine.
 *
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 */

void preorder_traversal(FILE *f, Tree tree)
{

  int i = 0;

  if (tree != NULL)
  {

    if (tree->manager == NULL)
    {
      fprintf(f, "%s ", tree->name);
    }
    else
      fprintf(f, "%s-%s ", tree->name, tree->manager->name);

    for (i = 0; i < tree->direct_employees_no; i++)
    {

      preorder_traversal(f, tree->team[i]);
    }
    if (tree->manager == NULL)
    {
      fprintf(f, "\n");
    }
  }
}

/* Elibereaza memoria alocata nodurilor din arbore
 *
 * tree: ierarhia existenta
 */

void destroy_tree(Tree tree)
{

  if (tree == NULL)
    return;

  for (int i = 0; i < tree->direct_employees_no; ++i)
  {
    destroy_tree(tree->team[i]);
  }

  free(tree->team);
  free(tree);
}
