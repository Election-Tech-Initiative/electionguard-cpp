#ifndef __ELECTIONGUARD_ENCRYPTION_COMPOSITOR_H_INCLUDED__
#define __ELECTIONGUARD_ENCRYPTION_COMPOSITOR_H_INCLUDED__

namespace electionguard
{
    class EncryptionCompositor
    {
      public:
        EncryptionCompositor();
        ~EncryptionCompositor();

        int encrypt();
    };

    int encrypt_ballot();
} // namespace electionguard

#endif /* __ELECTIONGUARD_ENCRYPTION_COMPOSITOR_H_INCLUDED__ */