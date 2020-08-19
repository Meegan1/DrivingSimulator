//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a RdGen v1.07.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------
#include "UnrealLogEvent.h"


namespace Jetbrains {
    namespace EditorPlugin {
        
        //companion
        
        //constants
        
        //initializer
        void UnrealLogEvent::initialize()
        {
        }
        
        //primary ctor
        UnrealLogEvent::UnrealLogEvent(rd::Wrapper<LogMessageInfo> info_, FString text_, TArray<rd::Wrapper<StringRange>> bpPathRanges_, TArray<rd::Wrapper<StringRange>> methodRanges_) :
        rd::IPolymorphicSerializable()
        ,info_(std::move(info_)), text_(std::move(text_)), bpPathRanges_(std::move(bpPathRanges_)), methodRanges_(std::move(methodRanges_))
        {
            initialize();
        }
        
        //secondary constructor
        
        //default ctors and dtors
        
        //reader
        UnrealLogEvent UnrealLogEvent::read(rd::SerializationCtx& ctx, rd::Buffer & buffer)
        {
            auto info_ = LogMessageInfo::read(ctx, buffer);
            auto text_ = rd::Polymorphic<FString>::read(ctx, buffer);
            auto bpPathRanges_ = buffer.read_array<TArray, StringRange, FDefaultAllocator>(
            [&ctx, &buffer]() mutable  
            { return StringRange::read(ctx, buffer); }
            );
            auto methodRanges_ = buffer.read_array<TArray, StringRange, FDefaultAllocator>(
            [&ctx, &buffer]() mutable  
            { return StringRange::read(ctx, buffer); }
            );
            UnrealLogEvent res{std::move(info_), std::move(text_), std::move(bpPathRanges_), std::move(methodRanges_)};
            return res;
        }
        
        //writer
        void UnrealLogEvent::write(rd::SerializationCtx& ctx, rd::Buffer& buffer) const
        {
            rd::Polymorphic<std::decay_t<decltype(info_)>>::write(ctx, buffer, info_);
            rd::Polymorphic<std::decay_t<decltype(text_)>>::write(ctx, buffer, text_);
            buffer.write_array<TArray, StringRange, FDefaultAllocator>(bpPathRanges_, 
            [&ctx, &buffer](StringRange const & it) mutable  -> void 
            { rd::Polymorphic<std::decay_t<decltype(it)>>::write(ctx, buffer, it); }
            );
            buffer.write_array<TArray, StringRange, FDefaultAllocator>(methodRanges_, 
            [&ctx, &buffer](StringRange const & it) mutable  -> void 
            { rd::Polymorphic<std::decay_t<decltype(it)>>::write(ctx, buffer, it); }
            );
        }
        
        //virtual init
        
        //identify
        
        //getters
        LogMessageInfo const & UnrealLogEvent::get_info() const
        {
            return *info_;
        }
        FString const & UnrealLogEvent::get_text() const
        {
            return text_;
        }
        TArray<rd::Wrapper<StringRange>> const & UnrealLogEvent::get_bpPathRanges() const
        {
            return bpPathRanges_;
        }
        TArray<rd::Wrapper<StringRange>> const & UnrealLogEvent::get_methodRanges() const
        {
            return methodRanges_;
        }
        
        //intern
        
        //equals trait
        bool UnrealLogEvent::equals(rd::ISerializable const& object) const
        {
            auto const &other = dynamic_cast<UnrealLogEvent const&>(object);
            if (this == &other) return true;
            if (this->info_ != other.info_) return false;
            if (this->text_ != other.text_) return false;
            if (this->bpPathRanges_ != other.bpPathRanges_) return false;
            if (this->methodRanges_ != other.methodRanges_) return false;
            
            return true;
        }
        
        //equality operators
        bool operator==(const UnrealLogEvent &lhs, const UnrealLogEvent &rhs) {
            if (lhs.type_name() != rhs.type_name()) return false;
            return lhs.equals(rhs);
        };
        bool operator!=(const UnrealLogEvent &lhs, const UnrealLogEvent &rhs){
            return !(lhs == rhs);
        }
        
        //hash code trait
        size_t UnrealLogEvent::hashCode() const noexcept
        {
            size_t __r = 0;
            __r = __r * 31 + (rd::hash<LogMessageInfo>()(get_info()));
            __r = __r * 31 + (rd::hash<FString>()(get_text()));
            __r = __r * 31 + (rd::contentDeepHashCode(get_bpPathRanges()));
            __r = __r * 31 + (rd::contentDeepHashCode(get_methodRanges()));
            return __r;
        }
        
        //type name trait
        std::string UnrealLogEvent::type_name() const
        {
            return "UnrealLogEvent";
        }
        
        //static type name trait
        std::string UnrealLogEvent::static_type_name()
        {
            return "UnrealLogEvent";
        }
        
        //polymorphic to string
        std::string UnrealLogEvent::toString() const
        {
            std::string res = "UnrealLogEvent\n";
            res += "\tinfo = ";
            res += rd::to_string(info_);
            res += '\n';
            res += "\ttext = ";
            res += rd::to_string(text_);
            res += '\n';
            res += "\tbpPathRanges = ";
            res += rd::to_string(bpPathRanges_);
            res += '\n';
            res += "\tmethodRanges = ";
            res += rd::to_string(methodRanges_);
            res += '\n';
            return res;
        }
        
        //external to string
        std::string to_string(const UnrealLogEvent & value)
        {
            return value.toString();
        }
    };
};