#ifndef MEERKAT_H_
#define MEERKAT_H_

enum Priority { HIGH, MEDIUM, LOW }

class Meerkat {
public:
  /* The buddy meerkat to send packets to. */
  Meerkat buddy;

  /* Add information to backup to the meerkat colony with a specified
  priority. */
  void add_to_colony(int *information, Priority priority);

  /* Designate the buddy meerkat to send packets to when loss is experienced. */
  void designate_buddy(Meerkat meerkat);

  /* Callback function for when the system loses power. Begins the process of
  sending packets to the buddy meerkat. Backs up the data in the order specified
  by their priorities. */
  void initiate_backup();

  /* Receives a packet from a buddy meerkat that has initiated loss mode. */
  void assist_meerkat();
};

#endif
