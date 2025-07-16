#define REGISTER_COMPONENT_TYPE(RegistrarName, Type, name_str)             \
    namespace                                                              \
    {                                                                      \
        struct RegistrarName##Registrar                                    \
        {                                                                  \
            RegistrarName##Registrar()                                     \
            {                                                              \
                Engine::ComponentRegistry::instance()                      \
                    .register_type<Type>(name_str);                        \
            }                                                              \
        };                                                                 \
        static RegistrarName##Registrar global_##RegistrarName##Registrar; \
    }