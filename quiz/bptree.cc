#include "bptree.h"
#include <vector>
#include <sys/time.h>

void
print_tree_core(NODE *n)
{
	printf("[");
	for (int i = 0; i < n->nkey; i++) {
		if (!n->isLeaf) print_tree_core(n->chi[i]);
		printf("%d", n->key[i]);
		if (i != n->nkey-1 && n->isLeaf) putchar(' ');
	}
	if (!n->isLeaf) print_tree_core(n->chi[n->nkey]);
	printf("]");
}

void
print_tree(NODE *node)
{
	print_tree_core(node);
	printf("\n"); fflush(stdout);
}

NODE *
alloc_leaf(NODE *parent)
{
	NODE *node;
	if (!(node = (NODE *)calloc(1, sizeof(NODE)))) ERR;
	node->isLeaf = true;
	node->parent = parent;
	node->nkey = 0;

	return node;
}

NODE *
alloc_internal(NODE *parent)
{
	NODE *node;
	if (!(node = (NODE *)calloc(1, sizeof(NODE)))) ERR;
	node->isLeaf = false;
	node->parent = parent;
	node->nkey = 0;

	return node;
}

NODE *
alloc_root(NODE *left, int rs_key, NODE *right)
{
	NODE *node;

	if (!(node = (NODE *)calloc(1, sizeof(NODE)))) ERR;
	node->parent = NULL;
	node->isLeaf = false;
	node->key[0] = rs_key;
	node->chi[0] = left;
	node->chi[1] = right;
	node->nkey = 1;

	return node;
}

NODE *
find_leaf(NODE *node, int key)
{
	int kid;

	if (node->isLeaf) return node;
	for (kid = 0; kid < node->nkey; kid++) {
		if (key < node->key[kid]) break;
	}

	return find_leaf(node->chi[kid], key);
}

NODE *
insert_in_leaf(NODE *leaf, int key, DATA *data)
{
	int i;
	if (key < leaf->key[0]) {
		for (i = leaf->nkey; i>0; i--) {
			leaf->chi[i] = leaf->chi[i-1];
			leaf->key[i] = leaf->key[i-1];
		}
		leaf->key[0] = key;
		leaf->chi[0] = (NODE *)data;
	}
	else { // Quiz
    // Step 2. Insert the new key
		for (i = leaf->nkey; leaf->key[i-1] > key; i--) {
			leaf->chi[i] = leaf->chi[i-1];
			leaf->key[i] = leaf->key[i-1];
		}
		leaf->key[i] = key;
		leaf->chi[i] = (NODE *)data;
	}
  // Step 1. Increment the number of keys
	leaf->nkey = leaf->nkey + 1;

	return leaf;
}

void
insert(int key, DATA *data)
{
	NODE *leaf;

	if (Root == NULL) {
		leaf = alloc_leaf(NULL);
		Root = leaf;
	}
	else {
		leaf = find_leaf(Root, key);
	}

	if (leaf->nkey < (N-1)) {
		insert_in_leaf(leaf, key, data);
	}
	else {
    // Split (quiz at 10/09)

		NODE *splited_leaf;
		splited_leaf = alloc_leaf(leaf);

		TEMP *keydata;

		for(int i=0; i<N; i++) {
			keydata->key[i] = leaf->key[i];
			keydata->chi[i] = leaf->chi[i];
		}
		insert_in_leaf(keydata, key, data);
		//printf("keydata: [%d, %d, %d, %d]", keydata->key[0], keydata->key[1], keydata->key[2], keydata->key[3]);

		splited_leaf->chi[N-1] = leaf->chi[N-1];
		leaf->chi[N-1] = splited_leaf;
		for(int i=0; i<leaf->nkey; i++) {
			leaf->key[i] = 0;
			leaf->chi[i] = NULL;
		}
		leaf->nkey = 0;

		for(int i=0; i<N/2; i++) {
			leaf->chi[i] = keydata->chi[i];
			leaf->key[i] = keydata->key[i];
			leaf->nkey = leaf->nkey + 1;

			splited_leaf->chi[i] = keydata->chi[i+(N/2)];
			splited_leaf->key[i] = keydata->key[i+(N/2)];
			splited_leaf->nkey = splited_leaf->nkey + 1;
		}

		int key_ = splited_leaf->key[0];
		Root = alloc_root(leaf, key_, splited_leaf);
		print_tree_core(Root);

	}
}

void
init_root(void)
{
	Root = NULL;
}

void
search_core(const int key)
{
  NODE *n = find_leaf(Root, key);
	for (int i = 0; i < n->nkey+1; i++) {
		if (n->key[i] == key) return;
	}
  cout << "Key not found: " << key << endl;
	ERR;
}

int
interactive()
{
  int key;

  std::cout << "Key: ";
  std::cin >> key;

  return key;
}

int
main(int argc, char *argv[])
{
	init_root();

  while (true) {
		insert(interactive(), NULL);
    print_tree(Root);
  }

	return 0;
}
