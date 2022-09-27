namespace ElectionGuard
{
    /// <summary>
    /// Metadata for encryption device
    ///
    /// The encryption device is a stateful container that represents abstract hardware
    /// authorized to participate in a specific election.
    ///
    /// </summary>
    public class EncryptionDevice : EncryptionDeviceBase
    {
        /// <summary>
        /// Create a new EncryptionDevice
        /// </summary>
        /// <param name="deviceUuid">a unique identifier tied to the device hardware</param>
        /// <param name="sessionUuid">a unique identifier tied to the runtime session</param>
        /// <param name="launchCode">a unique identifer tied to the election</param>
        /// <param name="location">an arbitrary string meaningful to the external system
        ///                        such as a friendly name, description, or some other value</param>
        public unsafe EncryptionDevice(
            ulong deviceUuid,
            ulong sessionUuid,
            ulong launchCode,
            string location) : base(deviceUuid, sessionUuid, launchCode, location)
        {
        }

        public unsafe EncryptionDevice(string json) : base(json) { }
    }

    /// <summary>
    /// An object for caching election and encryption state.
    ///
    /// the encryption mediator composes ballots by querying the encryption device
    /// for a hash of its metadata and incremental timestamps/
    ///
    /// this is a convenience wrapper around the encrypt methods
    /// and may not be suitable for all use cases.
    /// </summary>
    public class EncryptionMediator : EncryptionMediatorBase
    {
        /// <summary>
        /// Create an `EncryptionMediator` object
        /// </summary>
        /// <param name="manifest"></param>
        /// <param name="context"></param>
        /// <param name="device"></param>
        public unsafe EncryptionMediator(
            InternalManifest manifest,
            CiphertextElectionContext context,
            EncryptionDevice device) : base(manifest, context, device)
        {
        }
    }

    /// <summary>
    /// Metadata for encryption
    ///
    /// The encrypt object is used for encrypting ballots.
    ///
    /// </summary>
    public class Encrypt : EncryptBase
    {

    }
}
