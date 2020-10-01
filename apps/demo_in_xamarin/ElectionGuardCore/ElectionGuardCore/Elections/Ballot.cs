using System;
using System.Collections.Generic;

namespace ElectionGuardCore.Elections
{
    /// <summary>
    /// A BallotSelection represents an individual selection on a ballot.
    /// 
    /// This class accepts a `vote` string field which has no constraints
    /// in the ElectionGuard Data Specification, but is constrained logically
    /// in the application to resolve to `True` or `False`.  This implies that the
    /// data specification supports passing any string that can be represented as
    /// an integer, however only 0 and 1 is supported for now.
    /// 
    /// This class can also be designated as `is_placeholder_selection` which has no
    /// context to the data specification but is useful for running validity checks internally
    /// 
    /// an `extended_data` field exists to support any arbitrary data to be associated
    /// with the selection.In practice, this field is the cleartext representation
    /// of a write-in candidate value.  In the current implementation these values are
    /// discarded when encrypting.
    /// </summary>
    public class PlaintextBallotSelection
    {
        // The SelectionDescription ObjectId
        public string ObjectId { get; set; }
        public string Vote { get; set; }

        // Determines if this is a placeholder selection
        public bool IsPlaceholderSelection { get; set; }

        // an optional field of arbitrary data, such as the value of a write-in candidate
        public string ExtendedData { get; set; }
    }

    /// <summary>
    /// A CiphertextBallotSelection represents an individual encrypted selection on a ballot.
    /// 
    /// This class accepts a `description_hash` and a `ciphertext` as required parameters
    /// in its constructor.
    /// 
    /// When a selection is encrypted, the `description_hash` and `ciphertext` required fields must
    /// be populated at construction however the `nonce` is also usually provided by convention.
    /// 
    /// After construction, the `crypto_hash` field is populated automatically in the `__post_init__` cycle
    /// 
    /// A consumer of this object has the option to discard the `nonce` and/or discard the `proof`,
    /// or keep both values.  
    /// 
    /// 
    /// By discarding the `nonce`, the encrypted representation and `proof`
    /// can only be regenerated if the nonce was derived from the ballot's master nonce.  If the nonce
    /// used for this selection is truly random, and it is discarded, then the proofs cannot be regenerated.
    /// 
    /// By keeping the `nonce`, or deriving the selection nonce from the ballot nonce, an external system can
    /// regenerate the proofs on demand.This is useful for storage or memory constrained systems.
    /// 
    /// By keeping the `proof` the nonce is not required fotor verify the encrypted selection.
    /// </summary>
    public class CiphertextBallotSelection
    {
        // The SelectionDescription ObjectId
        public string ObjectId { get; set; }

        // The SelectionDescription hash
        public ElementModQ DescriptionHash { get; set; }

        // The encrypted representation of the vote field
        public ElGamalCiphertext Ciphertext { get; set; }

        // The hash of the encrypted values
        public ElementModQ CryptoHash { get; set; }

        // Determines if this is a placeholder selection
        public bool IsPlaceholderSelection { get; set; }

        // The nonce used to generate the encryption. Sensitive & should be treated as a secret
        public ElementModQ Nonce { get; set; }

        // The proof that demonstrates the selection is an encryption of 0 or 1, and was encrypted using the `nonce`
        public DisjunctiveChaumPedersenProof Proof { get; set; }

        // encrypted representation of the extended_data field
        public ElGamalCiphertext ExtendedData { get; set; }
    }

    /// <summary>
    /// A PlaintextBallotContest represents the selections made by a voter for a specific ContestDescription
    /// 
    /// this class can be either a partial or a complete representation of a contest dataset.Specifically,
    /// a partial representation must include at a minimum the "affirmative" selections of a contest.
    /// A complete representation of a ballot must include both affirmative and negative selections of
    /// the contest, AND the placeholder selections necessary to satisfy the ConstantChaumPedersen proof
    /// in the CiphertextBallotContest.
    /// 
    /// Typically partial contests are passed into Electionguard for memory constrained systems,
    /// while complete contests are passed into ElectionGuard when running encryption on an existing dataset.
    /// </summary>
    public class PlaintextBallotContest
    {
        // The ContestDescription ObjectId
        public string ObjectId { get; set; }
        public List<PlaintextBallotSelection> BallotSelections { get; set; }
    }

    /// <summary>
    /// A CiphertextBallotContest represents the selections made by a voter for a specific ContestDescription
    /// 
    /// CiphertextBallotContest can only be a complete representation of a contest dataset.While
    /// PlaintextBallotContest supports a partial representation, a CiphertextBallotContest includes all data
    /// necessary for a verifier to verify the contest.Specifically, it includes both explicit affirmative
    /// and negative selections of the contest, as well as the placeholder selections that satisfy
    /// the ConstantChaumPedersen proof.
    /// 
    /// Similar to `CiphertextBallotSelection` the consuming application can choose to discard or keep both
    /// the `nonce` and the `proof` in some circumstances.  For deterministic nonce's derived from the 
    /// master nonce, both values can be regenerated.If the `nonce` for this contest is completely random,
    /// then it is required in order to regenerate the proof.
    /// </summary>
    public class CiphertextBallotContest
    {
        // The ContestDescription ObjectId
        public string ObjectId { get; set; }

        // Hash from contestDescription
        public ElementModQ DescriptionHash { get; set; }


        public List<CiphertextBallotSelection> BallotSelections { get; set; }

        // Hash of the encrypted values
        public ElementModQ CryptoHash { get; set; }

        // The nonce used to generate the encryption. Sensitive & should be treated as a secret
        public ElementModQ Nonce { get; set; }

        // The proof demonstrates the sum of the selections does not exceed the maximum
        // available selections for the contest, and that the proof was generated with the nonce
        public ConstantChaumPedersenProof Proof { get; set; }
    }

    /// <summary>
    /// A PlaintextBallot represents a voters selections for a given ballot and ballot style
    /// </summary>
    public class PlaintextBallot
    {
        // A Unique ObjectId relevant to the external system
        // usually a GUID
        public string ObjectId { get; set; }

        // The `object_id` of the `BallotStyle` in the `Election` Manifest
        public string BallotStyle { get; set; }
        public List<PlaintextBallotContest> Contests { get; set; }
    }

    /// <summary>
    /// A CiphertextBallot represents a voters encrypted selections for a given ballot and ballot style.
    /// 
    /// When a ballot is in it's complete, encrypted state, the `nonce` is the master nonce
    /// from which all other nonces can be derived to encrypt the ballot.Allong with the `nonce`
    /// fields on `Ballotcontest` and `BallotSelection`, this value is sensitive.
    /// </summary>
    public class CiphertextBallot
    {
        // A Unique ObjectId relevant to the external system
        // usually a GUID
        public string ObjectId { get; set; }

        // The `object_id` of the `BallotStyle` in the `Election` Manifest
        public string BallotStyle { get; set; }

        // Hash of the election metadata
        public ElementModQ DescriptionHash { get; set; }

        // Previous tracking hash or seed hash
        public ElementModQ PreviousTrackingHash { get; set; }

        public List<CiphertextBallotContest> Contests { get; set; }

        // Unique ballot tracking hash for this ballot
        public ElementModQ TrackingHash { get; set; }

        // A tracker hash as a code in friendly readable words for sharing
        // Do Not send to API
        // TODO: remove setter
        public string TrackingCode { get; set; }

        // Timestamp at which the ballot encryption is generated in tick
        public ulong Timestamp { get; set; }

        // The hash of the encrypted ballot representation
        public ElementModQ CryptoHash { get; set; }

        // The nonce used to encrypt this ballot. Sensitive & should be treated as a secret
        public ElementModQ Nonce { get; set; }

    }

    public enum BallotBoxState
    {
        CAST = 1,
        SPOILED = 2,
        UNKNOWN = 999
    }

    /// <summary>
    /// A `CiphertextAcceptedBallot` represents a ballot that is accepted for inclusion in election results.
    /// An accepted ballot is or is about to be either cast or spoiled.
    /// The state supports the `BallotBoxState.UNKNOWN` enumeration to indicate that this object is mutable
    /// and has not yet been explicitly assigned a specific state.
    /// 
    /// Note, additionally, this ballot includes all proofs but no nonces.
    /// </summary>
    public class CiphertextAcceptedBallot : CiphertextBallot
    {
        public BallotBoxState State { get; set; }
    }

}
