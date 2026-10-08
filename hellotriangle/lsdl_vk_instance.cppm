export module lsdl_vk_instance;

export import :instance;
export import :debug_messenger;

// NOTE: validation is not an exporting interface, so it isn't mentioned here.
// The validation partition only defines the compile time data that is needed
// for this module. This is mimicing how I'd have headers that define data that
// I can easily include and reuse in multiple related files. In modules, this
// would be a partition that is only imported in the implementation files (cpp
// files).
// We should not import non-exporting partitions in exporting partitions, since
// this will result in the data not being exported and available to use by the
// customer (the .lib file for exampje).
