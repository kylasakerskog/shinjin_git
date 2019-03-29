#include <linux/module.h>
#include <linux/netlink.h>
#include <linux/init.h>
#include <net/sock.h>

#define NETLINK_TEST 17

static struct sock *nl_sock;
char *msg=";"; //message


static void nl_recv_msg (struct sk_buff *skb_in) { //skb_inは入力バッファ
  struct nlmsghdr *nlh; //ヘッダ
  struct sk_buff *skb_out; //出力バッファ
  int msg_size = 0; //メッセージのサイズ
  int pid = 0; //ポート番号?

  //ユーザプロセスから受信
  nlh = (struct nlmsghdr *) skb_in->data;
  pid = nlh->nlmsg_pid; //nlmsg_pidにポート番号は含まれているっぽい
  //TODO:
  //*msg = ;
  //printk("%s",msg); //カーネルに出力
  //メッセージがどこに出力されるのか調べる必要あり

  //ユーザプロセスに送信
  msg_size = strlen(msg);
  skb_out = nlmsg_new(msg_size, 0); //新しいnetlink messageを作成,第2引数はflag
  nlh = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, msg_size, 0); //第2引数はネットリンクのプロセスID，第3引数はシーケンス番号，第4引数はメッセージタイプ，第6引数はflag
  NETLINK_CB(skb_out).dst_group = 0; //わからん
  strncpy(nlmsg_data(nlh), msg, msg_size); //nlhのポインタ先に書き込む
  nlmsg_unicast(nl_sock, skb_out, pid); //ユニキャスト
}

struct netlink_kernel_cfg cfg = {
  .input = nl_recv_msg,
};

static int __init netlink_module_init(void){
  //printk(KERN_INFO "NETLINK TEST START\n");
  nl_sock = netlink_kernel_create(&init_net, NETLINK_TEST, &cfg); //kernel作成，引数は3つ(バージョンによっては6つ)
  //NETLINK_TESTはnetwork.hで未定義な値(17)を利用．引数がわからん？？
  return 0;
}

static void __exit netlink_module_exit(void)
{
  printk(KERN_INFO "NETLINK TEST FINISH\n");
  netlink_kernel_release(nl_sock);
}

module_init(netlink_module_init);
module_exit(netlink_module_exit);

MODULE_AUTHOR("hana_shin");
MODULE_LICENSE("GPL v2");
