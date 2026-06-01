# RenderPass vs Dynamic Rendering

## Understanding as of the Overview step of the tutorial

It hasn't yet shown us how the dynamic rendering command is structured, so
there's a lot of guesswork right now.

Render passes used to be fixed configurations of how we ran the shaders and what
attachments we sent to and read from each subpass. It also needed us to record
the set of commands for each frame.

Dynamic Rendering apparently gets rid of this entirely by making it a single
"beginRendering" command that takes the attachment info. I don't fully
understand how it's better, but getting rid of the entire RenderPass and
AttachmentDescription/AttachmentReference stuff would be nice. I wonder how
the attachment optimizations work in this case. We used to specify what each
attachment was used for to allow Vulkan to optimize the ops.
